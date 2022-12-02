#ifndef HISTORIES_H
#define HISTORIES_H

enum HistoryType{
        phase_v,
        velocity_,
        dist_v,
        acceleration_,
        velocity_a,
        dist_a,
        phase_acc,
        time_stamp
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
        time_stamp.push_back(0.0);

    };
    ~Histories(){};
    list<double> phase_history_;
    list<double> velocity_history_;
    list<double> dist_history_;
    list<double> acc_history_;
    list<double> acc2velocity_history_;
    list<double> acc2dist_history_;
    list<double> acc_phase_history_;
    list<double> time_stamp;

    void check_size(){
        int len;
        int max_len = 40960;
        len = phase_history_.size();
        while(len > max_len){
            phase_history_.pop_front();
            len--;
        }

        len = velocity_history_.size();
        while(len > max_len){
            velocity_history_.pop_front();
            len--;
        }

        len = acc_history_.size();
        while(len > max_len){
            acc_history_.pop_front();
            len--;
        }

        len = acc2velocity_history_.size();
        while(len > max_len){
            acc2velocity_history_.pop_front();
            len--;
        }

        len = acc2dist_history_.size();
        while(len > max_len){
            acc2dist_history_.pop_front();
            len--;
        }

        len = acc_phase_history_.size();
        while(len > max_len){
            acc_phase_history_.pop_front();
            len--;
        }

        len = time_stamp.size();
        while(len > max_len){
            time_stamp.pop_front();
            len--;
        }
    }

    int getLength(){
        return time_stamp.size();
    }

    void clearbuff(){
        phase_history_.clear();
        velocity_history_.clear();
        acc_history_.clear();
        acc2velocity_history_.clear();
        acc2dist_history_.clear();
        acc_phase_history_.clear();
        dist_history_.clear();
        time_stamp.clear();

    }

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