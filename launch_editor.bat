@echo off
setlocal EnableDelayedExpansion
if %processor_architecture% == AMD64 (
	set arch=x64
	if not exist bin\main_!arch!.exe (
		set arch=x86
	) else (
		set PATH=x64;bin\x64;!PATH!;
	)
) else (
	set arch=x86
)
start bin\main_%arch%.exe  -video_app auto -video_vsync 0 -video_refresh 0 -video_multisample 0 -video_gamma 1.0 -video_mode 1 -video_resizable 0 -video_fullscreen 0 -sound_app auto -data_path "../"  -engine_config "../data/FPS_Demo1/unigine.cfg" -engine_log "log.html" -system_script "FPS_Demo1/unigine.cpp" -editor_script "editor/editor.cpp" -gui_path "core/gui/" -extern_plugin "Collada","OpenFlight","Autodesk","Interface" -extern_define EDITOR -console_command "config_readonly 0 && world_load \"FPS_Demo1/FPS_Demo1\" && editor_reload"