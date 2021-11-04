#ifndef HISTORIES_H
#define HISTORIES_H


class Histories {

public:
    

    Histories(){
        phase_history_.push_back(0.0);
        velocity_history_.push_back(0.0);
        dist_history_.push_back(0.0);
    };
    ~Histories(){};
    vector<double> phase_history_;
    vector<double> velocity_history_;
    vector<double> dist_history_;

};

class Pre_Data_Store {

public:
    Pre_Data_Store(){
        complex<double> pre_tap_(0, 0);
        double pre_phase_in_wrap_ = 0.0;
    };
    ~Pre_Data_Store(){};
    complex<double> pre_tap_;
    double pre_phase_in_wrap_;
    
};




#endif