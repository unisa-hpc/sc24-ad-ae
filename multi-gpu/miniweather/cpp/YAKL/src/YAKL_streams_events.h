
#pragma once

namespace yakl {

  #ifdef YAKL_ENABLE_STREAMS
    bool constexpr streams_enabled = true;
  #else
    /** @brief If the CPP Macro YAKL_ENABLE_STREAMS is defined, then this bool is set to `true` */
    bool constexpr streams_enabled = false;
  #endif

  #if   defined(YAKL_ARCH_CUDA)

    class Stream;
    class Event;

    class Stream {
      protected:
      cudaStream_t my_stream;
      int *        refCount;       // Pointer shared by multiple copies of this Array to keep track of allcation / free

      void nullify() { my_stream = 0; refCount = nullptr; }

      public:

      Stream() { nullify(); }
      Stream(cudaStream_t cuda_stream) { nullify(); my_stream = cuda_stream; }
      ~Stream() { destroy(); }

      Stream(Stream const  &rhs) {
        my_stream = rhs.my_stream;
        refCount  = rhs.refCount;
        if (refCount != nullptr) (*refCount)++;
      }
      Stream(Stream &&rhs) {
        my_stream = rhs.my_stream;
        refCount  = rhs.refCount;
        rhs.nullify();
      }
      Stream & operator=(Stream const  &rhs) {
        if (this != &rhs) {
          destroy();
          my_stream = rhs.my_stream;
          refCount  = rhs.refCount;
          if (refCount != nullptr) (*refCount)++;
        }
        return *this;
      }
      Stream & operator=(Stream &&rhs) {
        if (this != &rhs) {
          destroy();
          my_stream = rhs.my_stream;
          refCount  = rhs.refCount;
          rhs.nullify();
        }
        return *this;
      }

      void create() {
        if (refCount == nullptr) {
          refCount = new int;
          (*refCount) = 1;
          if constexpr (streams_enabled) cudaStreamCreate( &my_stream );
        }
      }

      void destroy() {
        if (refCount != nullptr) {
          (*refCount)--;
          if ( (*refCount) == 0 ) {
            if constexpr (streams_enabled) cudaStreamDestroy( my_stream );
            delete refCount;
            nullify();
          }
        }
      }

      cudaStream_t get_real_stream() { return my_stream; }
      bool operator==(Stream stream) const { return my_stream == stream.get_real_stream(); }
      inline void wait_on_event(Event event);
      bool is_default_stream() { return my_stream == 0; }
      void fence() { cudaStreamSynchronize(my_stream); }
    };


    class Event {
      protected:
      cudaEvent_t my_event;
      int *       refCount;       // Pointer shared by multiple copies of this Array to keep track of allcation / free

      void nullify() { my_event = 0; refCount = nullptr; }

      public:

      Event() { nullify(); }
      ~Event() { destroy(); }

      Event(Event const  &rhs) {
        my_event = rhs.my_event;
        refCount = rhs.refCount;
        if (refCount != nullptr) (*refCount)++;
      }
      Event(Event &&rhs) {
        my_event = rhs.my_event;
        refCount = rhs.refCount;
        rhs.nullify();
      }
      Event & operator=(Event const  &rhs) {
        if (this != &rhs) {
          destroy();
          my_event = rhs.my_event;
          refCount = rhs.refCount;
          if (refCount != nullptr) (*refCount)++;
        }
        return *this;
      }
      Event & operator=(Event &&rhs) {
        if (this != &rhs) {
          destroy();
          my_event = rhs.my_event;
          refCount = rhs.refCount;
          rhs.nullify();
        }
        return *this;
      }

      void create() {
        if (refCount == nullptr) {
          refCount = new int;
          (*refCount) = 1;
          cudaEventCreate( &my_event );
        }
      }

      void destroy() {
        if (refCount != nullptr) {
          (*refCount)--;
          if ( (*refCount) == 0 ) { cudaEventDestroy( my_event ); delete refCount; nullify(); }
        }
      }

      inline void record(Stream stream);
      cudaEvent_t get_real_event() { return my_event; }
      bool operator==(Event event) const { return my_event == event.get_real_event(); }
      bool completed() { return cudaEventQuery( my_event ) == cudaSuccess; }
      void fence() { cudaEventSynchronize(my_event); }
    };


    inline void Event::record(Stream stream) {
      create();
      cudaEventRecord( my_event , stream.get_real_stream() );
    }


    inline void Stream::wait_on_event(Event event) {
      cudaStreamWaitEvent( my_stream , event.get_real_event() , 0 );
    }

  #elif defined(YAKL_ARCH_HIP)

    class Stream;
    class Event;

    class Stream {
      protected:
      hipStream_t my_stream;
      int *        refCount;       // Pointer shared by multiple copies of this Array to keep track of allcation / free

      void nullify() { my_stream = 0; refCount = nullptr; }

      public:

      Stream() { nullify(); }
      Stream(hipStream_t hip_stream) { nullify(); my_stream = hip_stream; }
      ~Stream() { destroy(); }

      Stream(Stream const  &rhs) {
        my_stream = rhs.my_stream;
        refCount  = rhs.refCount;
        if (refCount != nullptr) (*refCount)++;
      }
      Stream(Stream &&rhs) {
        my_stream = rhs.my_stream;
        refCount  = rhs.refCount;
        rhs.nullify();
      }
      Stream & operator=(Stream const  &rhs) {
        if (this != &rhs) {
          destroy();
          my_stream = rhs.my_stream;
          refCount  = rhs.refCount;
          if (refCount != nullptr) (*refCount)++;
        }
        return *this;
      }
      Stream & operator=(Stream &&rhs) {
        if (this != &rhs) {
          destroy();
          my_stream = rhs.my_stream;
          refCount  = rhs.refCount;
          rhs.nullify();
        }
        return *this;
      }

      void create() {
        if (refCount == nullptr) {
          refCount = new int;
          (*refCount) = 1;
          if constexpr (streams_enabled) hipStreamCreate( &my_stream );
        }
      }

      void destroy() {
        if (refCount != nullptr) {
          (*refCount)--;
          if ( (*refCount) == 0 ) {
            if constexpr (streams_enabled) hipStreamDestroy( my_stream );
            delete refCount;
            nullify();
          }
        }
      }

      hipStream_t get_real_stream() { return my_stream; }
      bool operator==(Stream stream) const { return my_stream == stream.get_real_stream(); }
      inline void wait_on_event(Event event);
      bool is_default_stream() { return my_stream == 0; }
      void fence() { hipStreamSynchronize(my_stream); }
    };


    class Event {
      protected:
      hipEvent_t my_event;
      int *       refCount;       // Pointer shared by multiple copies of this Array to keep track of allcation / free

      void nullify() { my_event = 0; refCount = nullptr; }

      public:

      Event() { nullify(); }
      ~Event() { destroy(); }

      Event(Event const  &rhs) {
        my_event = rhs.my_event;
        refCount = rhs.refCount;
        if (refCount != nullptr) (*refCount)++;
      }
      Event(Event &&rhs) {
        my_event = rhs.my_event;
        refCount = rhs.refCount;
        rhs.nullify();
      }
      Event & operator=(Event const  &rhs) {
        if (this != &rhs) {
          destroy();
          my_event = rhs.my_event;
          refCount = rhs.refCount;
          if (refCount != nullptr) (*refCount)++;
        }
        return *this;
      }
      Event & operator=(Event &&rhs) {
        if (this != &rhs) {
          destroy();
          my_event = rhs.my_event;
          refCount = rhs.refCount;
          rhs.nullify();
        }
        return *this;
      }

      void create() {
        if (refCount == nullptr) {
          refCount = new int;
          (*refCount) = 1;
          hipEventCreate( &my_event );
        }
      }

      void destroy() {
        if (refCount != nullptr) {
          (*refCount)--;
          if ( (*refCount) == 0 ) { hipEventDestroy( my_event ); delete refCount; nullify(); }
        }
      }

      inline void record(Stream stream);
      hipEvent_t get_real_event() { return my_event; }
      bool operator==(Event event) const { return my_event == event.get_real_event(); }
      bool completed() { return hipEventQuery( my_event ) == hipSuccess; }
      void fence() { hipEventSynchronize(my_event); }
    };


    inline void Event::record(Stream stream) {
      create();
      hipEventRecord( my_event , stream.get_real_stream() );
    }


    inline void Stream::wait_on_event(Event event) {
      hipStreamWaitEvent( my_stream , event.get_real_event() , 0 );
    }

  #elif defined(YAKL_ARCH_SYCL)

    class Stream;
    class Event;

    class Stream {
      protected:
      std::shared_ptr<synergy::queue> my_stream;

      public:

      Stream() { }
      Stream(synergy::queue &sycl_queue) { my_stream = std::make_shared<synergy::queue>(sycl_queue); }
      ~Stream() { my_stream.reset(); }

      Stream(Stream const  &rhs) { my_stream = rhs.my_stream; }
      Stream(Stream       &&rhs) { my_stream = rhs.my_stream; }
      Stream & operator=(Stream const  &rhs) { if (this != &rhs) { my_stream = rhs.my_stream; }; return *this; }
      Stream & operator=(Stream       &&rhs) { if (this != &rhs) { my_stream = rhs.my_stream; }; return *this; }

      void create() {
        if constexpr (streams_enabled) {
          sycl::device dev(sycl::gpu_selector{});
          my_stream = std::make_shared<synergy::queue>( synergy::queue( dev , 
                                                    [](sycl::exception_list exceptions) {
                                                      for (std::exception_ptr const &e : exceptions) {
                                                        try {
                                                          std::rethrow_exception(e);
                                                        } catch (sycl::exception const &e) {
                                                          std::cerr << "Caught asynchronous SYCL exception:" << std::endl
                                                          << e.what() << std::endl
                                                          << "Exception caught at file:" << __FILE__
                                                          << ", line:" << __LINE__ << std::endl;
                                                        }
                                                      }
                                                    } , sycl::property_list{sycl::property::queue::in_order{}} ) );
        }
      }

      void destroy() { my_stream.reset(); }
      synergy::queue & get_real_stream() const { return my_stream ? *my_stream : sycl_default_stream(); }
      bool operator==(Stream stream) const { return get_real_stream() == stream.get_real_stream(); }
      inline void wait_on_event(Event event);
      bool is_default_stream() const { return get_real_stream() == sycl_default_stream(); }
      void fence() { my_stream->wait(); }
    };


    class Event {
      protected:
      sycl::event my_event;

      public:

      Event() { }
      ~Event() { }

      Event(Event const  &rhs) { my_event = rhs.my_event; }
      Event(Event       &&rhs) { my_event = rhs.my_event; }
      Event & operator=(Event const  &rhs) { if (this != &rhs) { my_event = rhs.my_event; }; return *this; }
      Event & operator=(Event       &&rhs) { if (this != &rhs) { my_event = rhs.my_event; }; return *this; }

      void create() { }
      void destroy() { }

      inline void record(Stream stream);
      sycl::event & get_real_event() { return my_event; }
      bool operator==(Event event) const { return my_event == event.get_real_event(); }
      bool completed() { return my_event.get_info<sycl::info::event::command_execution_status>() == sycl::info::event_command_status::complete; }
      void fence() { my_event.wait(); }
    };


    inline void Event::record(Stream stream) { my_event = stream.get_real_stream().single_task( [=] () {} ); }


    inline void Stream::wait_on_event(Event event) { my_stream->single_task( event.get_real_event() , [=] () {} ); }

  #else

    struct Stream;
    struct Event;

    /** @brief Implements the functionality of a stream for parallel kernel execution. If the `Stream::create()` method
      *        is not called on this object, then it is the default stream. */
    struct Stream {
      /** @brief Create the stream */
      void create() { }
      /** @brief Destroy the stream */
      void destroy() { }
      /** @brief Determine if this stream is the same as the passed stream */
      bool operator==(Stream stream) const { return true; }
      /** @brief Tell the stream to wait until the passed event completes before continuing work in the stream. */
      inline void wait_on_event(Event event);
      /** @brief Determine whether this stream is the default stream. */
      bool is_default_stream() { return true; }
      /** @brief Pause all CPU work until all existing work in this stream completes. */
      void fence() { }
    };

    /** @brief Implements the functionality of an event within a stream. The event is not created until the
      *        `Event::create()` function is called. */
    struct Event {
      /** @brief Create the event */
      void create() { }
      /** @brief Destroy the event */
      void destroy() { }
      /** @brief Record an event in the passed stream */
      inline void record(Stream stream);
      /** @brief Determine if this event is the same as the passed event */
      bool operator==(Event event) const { return true; }
      /** @brief Determine if this event has completed */
      bool completed() { return true; }
      /** @brief Pause all CPU work until this event has completed */
      void fence() { }
    };

    inline void Event::record(Stream stream) { }
    inline void Stream::wait_on_event(Event event) {  }

  #endif
    

  /** @brief Create and return a Stream object. It is guaranteed to not be the default stream */
  inline Stream create_stream() { Stream stream; stream.create(); return stream; }

  /** @brief Create, record, and return an event using the given stream */
  inline Event record_event(Stream stream = Stream()) { Event event; event.create(); event.record(stream); return event; }


  /** @brief Implements a list of Stream objects. 
    *        Needs to store a pointer to avoid construction on the device since Array objects need to store a
    *        list of streams on which they depend.
    * 
    * This purposely mimics a std::vector class's methods */
  struct StreamList {
    /** @private */
    std::vector<Stream> *list;
    /** @brief Create an empty stream list */
    YAKL_INLINE StreamList() {
      #if YAKL_CURRENTLY_ON_HOST()
        list = new std::vector<Stream>;
      #endif
    }
    YAKL_INLINE ~StreamList() {
      #if YAKL_CURRENTLY_ON_HOST()
        delete list;
      #endif
    }
    /** @brief Add a stream to the end of the list */
    void push_back(Stream stream) {
      yakl_mtx_lock();
      list->push_back(stream);
      yakl_mtx_unlock();
    }
    /** @brief Get the number of streams in the list */
    int size() const { return list->size(); }
    /** @brief Determine whether the list is empty (has no streams) */
    bool empty() const { return list->empty(); }
    /** @brief Access the stream at the requested index */
    Stream operator[] (int i) { return (*list)[i]; }
    /** @brief Get a std::vector of the streams in the list */
    std::vector<Stream> get_all_streams() const { return *list; }
  };

}


