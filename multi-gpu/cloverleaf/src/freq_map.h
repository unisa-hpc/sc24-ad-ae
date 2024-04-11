#ifndef KERNEL_MAP_HPP
#define KERNEL_MAP_HPP

#include <map>
#include <string>

class KernelMap {
public:
    static std::map<std::string, double>& getIntelMax1100FreqMap_PerKernel() {
        static std::map<std::string, double> intelMax1100FreqMap = {
           
            // Add more initializations if needed
        };
        return intelMax1100FreqMap;
    }
    
    static std::map<std::string, double>& getIntelMax1100FreqMap_PerApp() {
        static std::map<std::string, double> intelMax1100FreqMap = {
         
            // Add more initializations if needed
        };
        return intelMax1100FreqMap;
    }

    static std::map<std::string, double>& getNvidiaFreqMapPerApp() {
        static std::map<std::string, double> nvidiaFreqMap = {
                {"PdV_1",960},
                {"accelerate",960},
                {"advec_cell_1",960},
                {"advec_cell_2",960},
                {"advec_cell_3",960},
                {"advec_mom_1",960},
                {"advec_mom_2",960},
                {"advec_mom_3",960},
                {"advec_mom_4",960},
                {"advec_mom_5",960},
                {"build_field_1",960},
                {"build_field_3",960},
                {"build_field_4",960},
                {"build_field_5",960},
                {"build_field_6",960},
                {"build_field_7",960},
                {"build_field_8",960},
                {"calc_dt_1",960},
                {"clover_pack_message_right_1",960},
                {"clover_pack_message_top_1",960},
                {"clover_unpack_message_right_1",960},
                {"clover_unpack_message_top_1",960},
                {"flux_calc_1",960},
                {"generate_chunk_1",960},
                {"generate_chunk_2",960},
                {"ideal_gas_1",960},
                {"initialise_chunk_1",960},
                {"initialise_chunk_2",960},
                {"initialise_chunk_3",960},
                {"initialise_chunk_4",960},
                {"initialise_chunk_5",960},
                {"reset_field_1",960},
                {"reset_field_2",960},
                {"revert_1",960},
                {"update_halo",960},
                {"viscosity_1",960}
        };
        return nvidiaFreqMap;
    }

     static std::map<std::string, double>& getNvidiaFreqMapPerKernel() {
        static std::map<std::string, double> nvidiaFreqMap = {
                {"PdV_1",960},
                {"accelerate",960},
                {"advec_cell_1",1125},
                {"advec_cell_2",0},
                {"advec_cell_3",0},
                {"advec_mom_1",870},
                {"advec_mom_2",870},
                {"advec_mom_3",0},
                {"advec_mom_4",0},
                {"advec_mom_5",0},
                {"build_field_1",960},
                {"build_field_3",960},
                {"build_field_4",960},
                {"build_field_5",960},
                {"build_field_6",0},
                {"build_field_7",0},
                {"build_field_8",0},
                {"calc_dt_1",960},
                {"clover_pack_message_right_1",870},
                {"clover_pack_message_top_1",870},
                {"clover_unpack_message_right_1",870},
                {"clover_unpack_message_top_1",870},
                {"flux_calc_1",960},
                {"generate_chunk_1",960},
                {"generate_chunk_2",960},
                {"ideal_gas_1",960},
                {"initialise_chunk_1",870},
                {"initialise_chunk_2",870},
                {"initialise_chunk_3",870},
                {"initialise_chunk_4",870},
                {"initialise_chunk_5",870},
                {"reset_field_1",960},
                {"reset_field_2",960},
                {"revert_1",960},
                {"update_halo",960},
                {"viscosity_1",1125}
        
            // Add more initializations if needed
        };
        return nvidiaFreqMap;
    }

     static std::map<std::string, double>& getNvidiaFreqMapPerPhase() {
        static std::map<std::string, double> nvidiaFreqMap = {
            {"kernel3", 4.0},
            {"kernel4", 5.5}
            // Add more initializations if needed
        };
        return nvidiaFreqMap;
    }

    static std::map<std::string, double>& getAmdFreqMap() {
        static std::map<std::string, double> amdMI100FreqMap = {
            {"kernel5", 6.0},
            {"kernel6", 7.5}
            // Add more initializations if needed
        };
        return amdMI100FreqMap;
    }
};

#endif /* KERNEL_MAP_HPP */






