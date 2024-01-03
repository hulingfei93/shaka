-module(sensor).
-export([sensor_thread/2]).

sensor_thread(SensorId, WatcherPid) ->
	random:seed(now()),
	SleepTime = random:uniform(10000),
	timer:sleep(SleepTime),
	Measurement = random:uniform(11),
	if Measurement == 11 ->
		exit(anomalous_reading);
	true ->
		WatcherPid ! { SensorId, Measurement }
	end,
	sensor_thread(SensorId, WatcherPid).

