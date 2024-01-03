-module(watcher).
-export([start/0, watcher_thread/3]).

%% enter the number of sensors to start the program
start() ->
	{ok, [ SensorNum ]} = io:fread("enter the number of sensors> ", "~d"),
	if SensorNum > 0 ->
		create(0, SensorNum);
	true ->
		io:fwrite("sensor number must be great than zero~n"),
		start()
	end.

create(SensorId, SensorNum) ->
	EndId = SensorId + 10,
	if EndId >= SensorNum ->
		create_watcher(SensorId, SensorNum);
	true ->
		create_watcher(SensorId, EndId),
		create(EndId, SensorNum)
	end.

create_watcher(StartSensorId, EndSensorId) ->
	SensorList = [],
	_ = spawn(?MODULE, watcher_thread, [StartSensorId, EndSensorId, SensorList]).

%% create sensors for a watcher
watcher_thread(StartSensorId, EndSensorId, SensorList) ->
	if StartSensorId == EndSensorId ->
		io:fwrite("Watcher Starts. Sensor List: ~p~n", [SensorList]),
		watcher_thread_loop(SensorList);
	true ->
		{ NewSensorPid, _ } = spawn_monitor(sensor, sensor_thread, [StartSensorId, self()]),
		UpdateSensorList = lists:append(SensorList, [{ StartSensorId, NewSensorPid }]),
		watcher_thread(StartSensorId + 1, EndSensorId, UpdateSensorList)
	end.

%% watcher thread
%% update sensor list when a particular sensor is died restarted
%% receive message from sensor and print info
watcher_thread_loop(SensorList) ->
	receive
		{ 'DOWN', _, process, Pid, Reason } ->
			{ SensorId, _ } = lists:keyfind(Pid, 2, SensorList),
			io:fwrite("Sensor(~p) died. Reason: ~p~n", [SensorId, Reason]),
			{ NewPid, _ } = spawn_monitor(sensor, sensor_thread, [SensorId ,self()]),
			NewSensorList = lists:keyreplace(Pid, 2, SensorList, { SensorId, NewPid }),
			io:fwrite("Sensor(~p) restarted. Sensor List: ~p~n", [SensorId, NewSensorList]);
	
		{ SID, Measurement } ->
			io:fwrite("Sensor(~p) Measurement: ~p~n", [SID, Measurement]),
			NewSensorList = SensorList
	end,
	watcher_thread_loop(NewSensorList).

