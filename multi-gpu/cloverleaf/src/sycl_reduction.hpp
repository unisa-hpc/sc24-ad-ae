/*
 Crown Copyright 2012 AWE.

 This file is part of CloverLeaf.

 CloverLeaf is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the
 Free Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 CloverLeaf is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License along with
 CloverLeaf. If not, see http://www.gnu.org/licenses/.
 */


#ifndef CLOVERLEAF_SYCL_SYCL_REDUCTION_HPP
#define CLOVERLEAF_SYCL_SYCL_REDUCTION_HPP

#include <CL/sycl.hpp>
#include <iostream>
#include <utility>
#include "sycl_utils.hpp"

namespace clover {

	template<typename T, typename U, typename C>
	struct local_reducer {

		cl::sycl::accessor<T, 1,
				cl::sycl::access::mode::read_write,
				cl::sycl::access::target::local> local;

		C actual;
		cl::sycl::accessor<U, 1, cl::sycl::access::mode::read_write, cl::sycl::access::target::global_buffer> result;

		local_reducer(cl::sycl::handler &h, size_t size, C actual, cl::sycl::buffer<U, 1> &b) :
				local(cl::sycl::range<1>(size), h),
				actual(actual),
				result(b.template get_access<cl::sycl::access::mode::read_write>(h)) {}

		inline void drain(cl::sycl::id<1> lid, cl::sycl::id<1> gid) const { local[lid] = result[gid]; }

	};


	template<typename nameT,
			size_t dimension,
			class RangeTpe,
			class LocalType,
			class LocalAllocator = std::nullptr_t,
			class Empty= std::nullptr_t,
			class Functor= std::nullptr_t,
			class BinaryOp= std::nullptr_t,
			class Finaliser= std::nullptr_t,
			class RangeLengthFn = std::nullptr_t,
			class RangeIdFn = std::nullptr_t>
	static inline void par_reduce_nd_impl(synergy::queue&q,
	                                      RangeTpe range, RangeLengthFn lengthFn, RangeIdFn rangeIdFn,
	                                      LocalAllocator allocator,
	                                      Empty empty,
	                                      Functor functor,
	                                      BinaryOp combiner,
	                                      Finaliser finaliser) {

		auto dev = q.get_device();

		size_t dot_num_groups;
		size_t dot_wgsize;
		if (dev.is_cpu()) {
			dot_num_groups = dev.get_info<cl::sycl::info::device::max_compute_units>();
			dot_wgsize = dev.get_info<cl::sycl::info::device::native_vector_width_double>() * 2;

		} else {
			dot_num_groups = dev.get_info<cl::sycl::info::device::max_compute_units>() * 4;
			dot_wgsize = dev.get_info<cl::sycl::info::device::max_work_group_size>();
		}

		size_t N = lengthFn(range);
		dot_num_groups = std::min(N, dot_num_groups);

#ifdef SYCL_DEBUG
		std::cout << "RD: dot_wgsize=" << dot_wgsize << " dot_num_groups:" << dot_num_groups << " N=" << N << "\n";
#endif

		q.submit([=](cl::sycl::handler &h) mutable {
			auto ctx = allocator(h, dot_wgsize);
			h.parallel_for<nameT>(
					cl::sycl::nd_range<1>(dot_num_groups * dot_wgsize, dot_wgsize),
					[=](cl::sycl::nd_item<1> item) {

						size_t i = item.get_global_id(0);
						size_t li = item.get_local_id(0);
						size_t global_size = item.get_global_range()[0];

						empty(ctx, cl::sycl::id<1>(li));
						for (; i < N; i += global_size) {
							functor(ctx, cl::sycl::id<1>(li), rangeIdFn(cl::sycl::id<1>(i), range));
						}

						size_t local_size = item.get_local_range()[0]; // 8
						for (size_t offset = local_size / 2; offset > 0; offset /= 2) {
							item.barrier(cl::sycl::access::fence_space::local_space);
							if (li < offset) {
								combiner(ctx, cl::sycl::id<1>(li), cl::sycl::id<1>(li + offset));
							}
						}

						if (li == 0) {
							finaliser(ctx, item.get_group(0), cl::sycl::id<1>(0));
						}

					});
		});

		q.submit([=](cl::sycl::handler &h) mutable {
			auto ctx = allocator(h, dot_num_groups);
			h.parallel_for<class final_reduction>(
					cl::sycl::nd_range<1>(1, 1),
					[=](auto) {
						auto zero = cl::sycl::id<1>(0);
						empty(ctx, zero); // local[0] = empty
						for (size_t i = 0; i < dot_num_groups; ++i) {
							ctx.drain(cl::sycl::id<1>(i), cl::sycl::id<1>(i));
						}
						for (size_t i = 1; i < dot_num_groups; ++i) {
							combiner(ctx, zero, cl::sycl::id<1>(i)); // local[0] = local[0] |+| xs[i]
						}
						finaliser(ctx, 0, zero); // xs[0] = local[0]
					});
		});
#ifdef SYNC_KERNELS
		q.wait_and_throw();
#endif
#ifdef SYCL_DEBUG
		std::cout << "RD: done= " << N << "\n";
#endif
	}


	template<typename nameT,
			class LocalType,
			class LocalAllocator = std::nullptr_t,
			class Empty = std::nullptr_t,
			class Functor = std::nullptr_t,
			class BinaryOp = std::nullptr_t,
			class Finaliser = std::nullptr_t>
	static inline void par_reduce_2d(synergy::queue&q, const clover::Range2d &range,
	                                 LocalAllocator allocator,
	                                 Empty empty,
	                                 Functor functor,
	                                 BinaryOp combiner,
	                                 Finaliser finaliser) {
#ifdef SYCL_DEBUG
		std::cout << "par_reduce_2d " << range << "\n";
#endif
		par_reduce_nd_impl<nameT, 2, clover::Range2d,
				LocalType,
				LocalAllocator,
				Empty,
				Functor,
				BinaryOp,
				Finaliser
		>(q, range,
		  [](clover::Range2d r) { return r.sizeX * r.sizeY; },
		  [](cl::sycl::id<1> gid, clover::Range2d r) {
#ifdef SYCL_FLIP_2D
			  const size_t x = r.fromX + (gid[0] % (r.sizeY));
			  const size_t y = r.fromY + (gid[0] / (r.sizeY));
			  return cl::sycl::id<2>(y, x);
#else
			  const size_t x = r.fromX + (gid[0] % (r.sizeX));
			  const size_t y = r.fromY + (gid[0] / (r.sizeX));
			  return cl::sycl::id<2>(x, y);
#endif
		  },
		  allocator, empty, functor, combiner, finaliser);
	}

// applies a 1d reduction
	template<typename nameT,
			class LocalType,
			class LocalAllocator = std::nullptr_t,
			class Empty = std::nullptr_t,
			class Functor = std::nullptr_t,
			class BinaryOp = std::nullptr_t,
			class Finaliser = std::nullptr_t>
	static inline void par_reduce_1d(synergy::queue&q, const clover::Range1d &range,
	                                 LocalAllocator allocator,
	                                 Empty empty,
	                                 Functor functor,
	                                 BinaryOp combiner,
	                                 Finaliser finaliser) {
#ifdef SYCL_DEBUG
		std::cout << "par_reduce_1d " << range << "\n";
#endif
		par_reduce_nd_impl<nameT, 1, clover::Range1d,
				LocalType,
				LocalAllocator,
				Empty,
				Functor,
				BinaryOp,
				Finaliser
		>(q, range,
		  [](clover::Range1d r) { return r.size; },
		  [](cl::sycl::id<1> gid, clover::Range1d r) { return r.from + gid[0]; },
		  allocator, empty, functor, combiner, finaliser);
	}

}


#endif //CLOVERLEAF_SYCL_SYCL_REDUCTION_HPP
