#ifndef HISTORIES_H
#define HISTORIES_H

enum HistoryType{
        phase_v,
        velocity_,
        dist_v,
        acceleration_,
        velocity_a,
        dist_a,
        phase_acc
    };

class mvMedian_data{
public:
    mvMedian_data(){
        iter = 0;
        for (int i = 0; i < 5; i++) {
            buffer[i] = 0.0;
        }
    };
    ~mvMedian_data(){};

    // 不能直接读指针成员？
    double iter;
    double buffer[5];
};

class Histories {

public:

    Histories(){
        phase_history_.push_back(0.0);
        velocity_history_.push_back(0.0);
        dist_history_.push_back(0.0);
        acc_history_.push_back(0.0);
        acc2velocity_history_.push_back(0.0);
        acc2dist_history_.push_back(0.0);
        acc_phase_history_.push_back(0.0);

    };
    ~Histories(){};
    vector<double> phase_history_;
    vector<double> velocity_history_;
    vector<double> dist_history_;
    vector<double> acc_history_;
    vector<double> acc2velocity_history_;
    vector<double> acc2dist_history_;
    vector<double> acc_phase_history_;

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