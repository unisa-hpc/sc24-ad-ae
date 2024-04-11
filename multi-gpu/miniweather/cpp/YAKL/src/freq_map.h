#ifndef KERNEL_MAP_HPP
#define KERNEL_MAP_HPP

#include <map>
#include <string>

class KernelMap {
public:
    static std::map<std::string, double>& getIntelMax1100FreqMap_PerKernel() {
        static std::map<std::string, double> intelMax1100FreqMap = {
            {"compute_tendencies_x_1",250},
            {"compute_tendencies_x_2",250},
            {"compute_tendencies_z_1",1000},
            {"compute_tendencies_z_2",0},
            {"init_1",250},
            {"init_2",0},
            {"init_3",0},
            {"reductions_1",200},
            {"semi_discret_step_1",250},
            {"set_halo_values_x_2",250},
            {"set_halo_values_x_3",0},
            {"set_halo_values_x_4",0},
            {"set_halo_values_z_1",0}
            // Add more initializations if needed
        };
        return intelMax1100FreqMap;
    }
    
    static std::map<std::string, double>& getIntelMax1100FreqMap_PerApp() {
        static std::map<std::string, double> intelMax1100FreqMap = {
            {"compute_tendencies_x_1",250},
            {"compute_tendencies_x_2",250},
            {"compute_tendencies_z_1",250},
            {"compute_tendencies_z_2",250},
            {"init_1",250},
            {"init_2",250},
            {"init_3",250},
            {"reductions_1",250},
            {"semi_discret_step_1",250},
            {"set_halo_values_x_2",250},
            {"set_halo_values_x_3",250},
            {"set_halo_values_x_4",250},
            {"set_halo_values_z_1",250}
            // Add more initializations if needed
        };
        return intelMax1100FreqMap;
    }

    static std::map<std::string, double>& getNvidiaFreqMap_PerApp() {
        static std::map<std::string, double> nvidiaV100FreqMap = {
            {"compute_tendencies_x_1",960},
            {"compute_tendencies_x_2",960},
            {"compute_tendencies_z_1",960},
            {"compute_tendencies_z_2",960},
            {"init_1",960},
            {"init_2",960},
            {"init_3",960},
            {"reductions_1",960},
            {"semi_discret_step_1",960},
            {"set_halo_values_x_2",960},
            {"set_halo_values_x_3",960},
            {"set_halo_values_x_4",960},
            {"set_halo_values_z_1",960}
            // Add more initializations if needed
        };
        return nvidiaV100FreqMap;
    }
     static std::map<std::string, double>& getNvidiaFreqMap_PerKernel() {
        static std::map<std::string, double> nvidiaV100FreqMap = {
            {"compute_tendencies_x_1",960},
            {"compute_tendencies_x_2",0},
            {"compute_tendencies_z_1",960},
            {"compute_tendencies_z_2",0},
            {"init_1",960},
            {"init_2",0},
            {"init_3",0},
            {"reductions_1",960},
            {"semi_discret_step_1",960},
            {"set_halo_values_x_2",960},
            {"set_halo_values_x_3",0},
            {"set_halo_values_x_4",960},
            {"set_halo_values_z_1",0}
            // Add more initializations if needed
        };
        return nvidiaV100FreqMap;
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






