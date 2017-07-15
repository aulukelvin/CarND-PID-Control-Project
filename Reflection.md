# CarND-Controls-PID Reflection
Self-Driving Car Engineer Nanodegree Program

---

## PID controller
The PID controller is widely used in industrial controlling systems and in the self-driving cars as well. 

Generally speaking the P controller action proportionally to the error, which is the most basic controlling model. The problem of the P controller is that well be a gap between the controlled output and the set value, but if the gain was set large then the output will jitter around the set value. The P controller itself can't stabilize the output.

Then I controller is working on the integral of the error. So if there's any accumulated error then the I controller can pick it up and make sure the output can align with the set value. But the I controller can't reduce the jittering. 

The purpose of the D controller is to action proportionally to the error's changing rate so that the jittering behavior of the P and I controllers can be fixed.

## PID controllers in the CarND-PID-Control-Project
In this project I used two PID controllers: one for the steering angle control and another for the speed control.

My tuning process has two steps:

* Coarse tuning. I set all PID gains to 0, then first slowly increase Kp and make sure the car can response to the turning properly and shows slightly jittering on the track. Then I slowly increase Ki to make sure the car follow the track even better. Then slowly increase the Kd to control the jittering.

* Fine tuning. I started the tuning at 30mph and found out the model doesn't works well at higher speed. Measure the performance with bare eyes seems not enough to evaluate the performance. So I did simple mean and variance analysis of the CTE and got a better model. The best performance I got is mean 0.227953445 and variance 0.214353542.

## Further enhancement
Both the fine tuned model and the fine tuning process seem stopped working if the speed is over 50mph. Then I add a control of the speed if the CTE is too high. The function I used is like the following:

```C++
    speed_pid.UpdateError(speed - set_speed * (4.5 - std::abs(cte))/4.5); // slow down when the cte is high
```

The speed control can help a lot in high speed driving. The highest speed I have tried is 65mph. The car can runs through the whole track at that speed but the driving is already very unstable so I decrease the speed setting to 55mph as final submit.

## Discussion
Normally the I controller should based on the integral of the error from time 0 to t. But I just wondering if there's a bump of big error then the I controller will tend to overshoot for a long period to make the integral even. So in my code I deliberately tried to make the I controller slightly more focus on the recent error and slowly forget the unhappiness in the past. My code is like the following:
```C++
    i_error *= 0.99;
    i_error += cte;
```
The fading effect is very modest because the impact of an error will decrease to 5% after 300 steps.
 
## PS
After successfully passed this project I realized that the speed control can be better if putting the delta of the cte into consideration, i.e. slow down the speed evern further when the cte is increasing and speed up when the cte is decreasing. This strategy is just like how people driving. I added this into the code and found out the car can safely ran through the track at 65mph.

Then I added the measured angle into the model. The calculated steering value was purely based on the PID controller and the cte as input. Suppose the car is facing along with the track center line but has a cte to correct, then in the process of the PID controller correcting the cte, the car's driving angle is actually more and more increasing and reachs to the maximum when the car's cte is equals zero. So the driving angle is sorts of integral of the previous operations which can cause jittering effect of the car driving. So I deducted the measured driving angle from the steering_value and found out the car can drive significantly smoother. My final code can run at 68mph.
