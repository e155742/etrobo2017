/**
 * @file sumo.cpp
 * @brief ET相撲Neo
 *
 * @author Suguru Kouchi
 */
#include "sumo.hpp"
#include "robo_meta_datas.hpp"
#include "util.h"

namespace ie {

Sumo::Sumo(ev3api::SonarSensor& sonarSensor, Motion motion):
sonarSensor_(sonarSensor), motion_(motion) {
}

/**
 * �܇��ͨ���^����ޤǄӤ���ֹͣ
 *
 * @param n �Ф�ο礰��`��
 */
void Sumo::trainWait(int n) {
    motion_.raiseArm(45, 15); // ��·�˥��`�ब���ä�����ʤ��褦��
    motion_.stop();
    while (true) {
        int16_t distance = sonarSensor_.getDistance(); // ���x��y��
        if (0 < distance && distance < 60) {
            // Ŀ��ǰ��ͨ�äƤ���Τ�ͨ���^����ޤǴ��C
            while (sonarSensor_.getDistance() < 60) {}
            motion_.wait(TRAIN_WAIT_TIME);
            break;
        }
        if (distance < 60 * n) {
            break;
        }
    }
}

}
