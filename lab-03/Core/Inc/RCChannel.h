/*
 * RCChannel.h
 *
 *  Created on: May 15, 2024
 *      Author: Max
 */

#ifndef SRC_RCCHANNEL_H_
#define SRC_RCCHANNEL_H_

#include "stm32f4xx_hal.h"

class RCChannel {
public:
    RCChannel(
        TIM_HandleTypeDef* timer_handle,
        uint32_t timer_channel,
		int32_t RC_signal_period_us,
		uint32_t CPU_clock_speed_MHz,
        int32_t full_reverse_us_width,
        int32_t neutral_us_width,
        int32_t full_forward_us_width,
        int32_t rescale_forward_magnitude,
		int32_t rescale_reverse_magnitude
    );
    virtual ~RCChannel();
    void TIM_IC_CaptureCallback();


    // Public variables
    int32_t first_value = 0;
    int32_t second_value = 0;
    int32_t difference = 0;
    int32_t us_width = 0;
    int32_t value = 0;
    int32_t rescale_forward_magnitude;
    int32_t rescale_reverse_magnitude;

private:
    // Instiation arguments
    TIM_HandleTypeDef* timer_handle;
    uint32_t timer_channel;
    int32_t RC_signal_period_us;
    uint32_t CPU_clock_speed_MHz;
    int32_t full_reverse_us_width;
    int32_t neutral_us_width;
    int32_t full_forward_us_width;

    // Needed between methods
    uint32_t minuend;
    float mFactor;
    bool first_capture = true;
    int32_t reverse_range;
    int32_t forward_range;

};

#endif /* SRC_RCCHANNEL_H_ */
