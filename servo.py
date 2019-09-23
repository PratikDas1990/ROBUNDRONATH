def moveservo(x,y):
	from adafruit_servokit import ServoKit
	kit = ServoKit(channels=16)
	kit.servo[14].set_pulse_width_range(600, 2500)
	kit.servo[14].angle = x
	kit.servo[14].set_pulse_width_range(1000, 2000)
	kit.servo[15].angle = y
