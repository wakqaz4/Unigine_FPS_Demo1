/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_sounds.h
 * Desc:    Unigine editor
 * Version: 1.03
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

/*
 */
namespace Sound {
	
	/**************************************************************************\
	*
	* SoundSource
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Source {
		
		/*
		 */
		WidgetVBox main_vb;					// parameters vbox
		
		WidgetLabel sample_l;				// parameters
		WidgetCheckBox loop_cb;
		WidgetCheckBox occlusion_cb;
		WidgetEditLine source_mask_el;
		WidgetEditLine reverb_mask_el;
		WidgetEditLine pitch_el;
		WidgetSlider pitch_sl;
		WidgetEditLine gain_el;
		WidgetSlider gain_sl;
		WidgetEditLine room_rolloff_el;
		WidgetSlider room_rolloff_sl;
		WidgetEditLine air_absorption_el;
		WidgetSlider air_absorption_sl;
		WidgetEditLine min_distance_el;
		WidgetEditLine max_distance_el;
		WidgetEditLine cone_inner_angle_el;
		WidgetSlider cone_inner_angle_sl;
		WidgetEditLine cone_outer_angle_el;
		WidgetSlider cone_outer_angle_sl;
		WidgetEditLine cone_outer_gain_el;
		WidgetSlider cone_outer_gain_sl;
		WidgetEditLine cone_outer_gain_hf_el;
		WidgetSlider cone_outer_gain_hf_sl;
		
		WidgetButton play_b;				// buttons
		WidgetButton stop_b;
		
		SoundSource source;
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Source"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			source = node_cast(node);
			
			sample_l.setText(basename(source.getSampleName()));
			sample_l.setToolTip(source.getSampleName());
			
			setCallbackEnabled(GUI_CLICKED,0,(
				loop_cb,
				occlusion_cb
			));
			
			loop_cb.setChecked(source.getLoop());
			occlusion_cb.setChecked(source.getOcclusion());
			
			setCallbackEnabled(GUI_CLICKED,1,(
				loop_cb,
				occlusion_cb
			));
			
			source_mask_el.setText(editorFormatMask(source.getSourceMask()));
			reverb_mask_el.setText(editorFormatMask(source.getReverbMask()));
			
			pitch_el.setText(editorFormat(source.getPitch()));
			gain_el.setText(editorFormat(source.getGain()));
			room_rolloff_el.setText(editorFormat(source.getRoomRolloff()));
			air_absorption_el.setText(editorFormat(source.getAirAbsorption()));
			min_distance_el.setText(editorFormat(source.getMinDistance()));
			max_distance_el.setText(editorFormat(source.getMaxDistance()));
			cone_inner_angle_el.setText(editorFormat(source.getConeInnerAngle()));
			cone_outer_angle_el.setText(editorFormat(source.getConeOuterAngle()));
			cone_outer_gain_el.setText(editorFormat(source.getConeOuterGain()));
			cone_outer_gain_hf_el.setText(editorFormat(source.getConeOuterGainHF()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			play_b.setEnabled(source.isStopped());
			stop_b.setEnabled(source.isPlaying());
			
			source.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_MASK(source_mask,source,SourceMask,source);
		CLICKED_CALLBACK_MASK(source_mask,source,SourceMask,source,TR("Source mask"),"Nodes::Sound::Source::source_mask_changed")
		PRESSED_CALLBACK_MASK(reverb_mask,source,ReverbMask,source);
		CLICKED_CALLBACK_MASK(reverb_mask,source,ReverbMask,source,TR("Reverb mask"),"Nodes::Sound::Source::reverb_mask_changed")
		CLICKED_CALLBACK(loop,source,Loop,source)
		CLICKED_CALLBACK(occlusion,source,Occlusion,source)
		PRESSED_CALLBACK_SLIDER(pitch,source,Pitch,source,modify)
		PRESSED_CALLBACK_SLIDER(gain,source,Gain,source,modify)
		PRESSED_CALLBACK_SLIDER(room_rolloff,source,RoomRolloff,source,modify)
		PRESSED_CALLBACK_SLIDER(air_absorption,source,AirAbsorption,source,modify)
		PRESSED_CALLBACK_FLOAT(min_distance,source,MinDistance,source,modify)
		PRESSED_CALLBACK_FLOAT(max_distance,source,MaxDistance,source,modify)
		PRESSED_CALLBACK_SLIDER(cone_inner_angle,source,ConeInnerAngle,source,modify)
		PRESSED_CALLBACK_SLIDER(cone_outer_angle,source,ConeOuterAngle,source,modify)
		PRESSED_CALLBACK_SLIDER(cone_outer_gain,source,ConeOuterGain,source,modify)
		PRESSED_CALLBACK_SLIDER(cone_outer_gain_hf,source,ConeOuterGainHF,source,modify)
		
		/*
		 */
		void source_mask_changed(int mask) {
			source.setSourceMask(mask);
			source_mask_el.setText(editorFormatMask(source.getSourceMask()));
		}
		
		void reverb_mask_changed(int mask) {
			source.setReverbMask(mask);
			reverb_mask_el.setText(editorFormatMask(source.getReverbMask()));
		}
		
		/*
		 */
		void play_clicked() {
			source.play();
		}
		
		void stop_clicked() {
			source.stop();
		}
		
	} /* namespace Source */
	
	/**************************************************************************\
	*
	* SoundReveb
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Reverb {
		
		/*
		 */
		WidgetVBox main_vb;					// parameters vbox
		WidgetVBox reverb_vb;
		
		WidgetEditLine reverb_mask_el;		// parameters
		WidgetEditLine size_el[3];
		WidgetEditLine threshold_el[3];
		WidgetEditLine gain_el;
		WidgetSlider gain_sl;
		WidgetEditLine gain_lf_el;
		WidgetSlider gain_lf_sl;
		WidgetEditLine gain_hf_el;
		WidgetSlider gain_hf_sl;
		WidgetEditLine room_rolloff_el;
		WidgetSlider room_rolloff_sl;
		WidgetEditLine air_absorption_el;
		WidgetSlider air_absorption_sl;
		WidgetEditLine density_el;
		WidgetSlider density_sl;
		WidgetEditLine diffusion_el;
		WidgetSlider diffusion_sl;
		WidgetEditLine echo_time_el;
		WidgetSlider echo_time_sl;
		WidgetEditLine echo_depth_el;
		WidgetSlider echo_depth_sl;
		WidgetEditLine modulation_time_el;
		WidgetSlider modulation_time_sl;
		WidgetEditLine modulation_depth_el;
		WidgetSlider modulation_depth_sl;
		WidgetEditLine decay_time_el;
		WidgetSlider decay_time_sl;
		WidgetEditLine decay_lf_ratio_el;
		WidgetSlider decay_lf_ratio_sl;
		WidgetEditLine decay_hf_ratio_el;
		WidgetSlider decay_hf_ratio_sl;
		WidgetEditLine reflection_gain_el;
		WidgetSlider reflection_gain_sl;
		WidgetEditLine reflection_delay_el;
		WidgetSlider reflection_delay_sl;
		WidgetEditLine late_reverb_gain_el;
		WidgetSlider late_reverb_gain_sl;
		WidgetEditLine late_reverb_delay_el;
		WidgetSlider late_reverb_delay_sl;
		WidgetComboBox preset_cb;
		
		SoundReverb reverb;
		
		#include <editor/editor_sounds_reverb.h>
		
		/*
		 */
		void init() {
			
			parameters_tb.addTab(TR("Reverb"));
			parameters_tb.addChild(main_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(main_vb);
			
			parameters_tb.addTab(TR("Params"));
			parameters_tb.addChild(reverb_vb,GUI_ALIGN_EXPAND);
			enableCallbacks(reverb_vb);
			
			preset_cb.clear();
			preset_cb.addItem(TR("Select preset"));
			foreach(string name; preset_names) {
				preset_cb.addItem(name);
			}
		}
		
		void shutdown() {
			
		}
		
		/*
		 */
		void update() {
			
			reverb = node_cast(node);
			
			reverb_mask_el.setText(editorFormatMask(reverb.getReverbMask()));
			
			setEditLineVec3(size_el,reverb.getSize());
			setEditLineVec3(threshold_el,reverb.getThreshold());
			
			gain_el.setText(editorFormat(reverb.getGain()));
			gain_lf_el.setText(editorFormat(reverb.getGainLF()));
			gain_hf_el.setText(editorFormat(reverb.getGainHF()));
			room_rolloff_el.setText(editorFormat(reverb.getRoomRolloff()));
			air_absorption_el.setText(editorFormat(reverb.getAirAbsorption()));
			density_el.setText(editorFormat(reverb.getDensity()));
			diffusion_el.setText(editorFormat(reverb.getDiffusion()));
			echo_time_el.setText(editorFormat(reverb.getEchoTime()));
			echo_depth_el.setText(editorFormat(reverb.getEchoDepth()));
			modulation_time_el.setText(editorFormat(reverb.getModulationTime()));
			modulation_depth_el.setText(editorFormat(reverb.getModulationDepth()));
			decay_time_el.setText(editorFormat(reverb.getDecayTime()));
			decay_lf_ratio_el.setText(editorFormat(reverb.getDecayLFRatio()));
			decay_hf_ratio_el.setText(editorFormat(reverb.getDecayHFRatio()));
			reflection_gain_el.setText(editorFormat(reverb.getReflectionGain()));
			reflection_delay_el.setText(editorFormat(reverb.getReflectionDelay()));
			late_reverb_gain_el.setText(editorFormat(reverb.getLateReverbGain()));
			late_reverb_delay_el.setText(editorFormat(reverb.getLateReverbDelay()));
		}
		
		/*
		 */
		void update_dynamic() {
			
			reverb.renderVisualizer();
		}
		
		/*
		 */
		PRESSED_CALLBACK_MASK(reverb_mask,reverb,ReverbMask,reverb);
		CLICKED_CALLBACK_MASK(reverb_mask,reverb,ReverbMask,reverb,TR("Reverb mask"),"Nodes::Sound::Reverb::reverb_mask_changed")
		PRESSED_CALLBACK_VEC3(size,reverb,Size,reverb,modify)
		PRESSED_CALLBACK_VEC3(threshold,reverb,Threshold,reverb,modify)
		PRESSED_CALLBACK_SLIDER(gain,reverb,Gain,reverb,modify)
		PRESSED_CALLBACK_SLIDER(gain_lf,reverb,GainLF,reverb,modify)
		PRESSED_CALLBACK_SLIDER(gain_hf,reverb,GainHF,reverb,modify)
		PRESSED_CALLBACK_SLIDER(room_rolloff,reverb,RoomRolloff,reverb,modify)
		PRESSED_CALLBACK_SLIDER(air_absorption,reverb,AirAbsorption,reverb,modify)
		PRESSED_CALLBACK_SLIDER(density,reverb,Density,reverb,modify)
		PRESSED_CALLBACK_SLIDER(diffusion,reverb,Diffusion,reverb,modify)
		PRESSED_CALLBACK_SLIDER(echo_time,reverb,EchoTime,reverb,modify)
		PRESSED_CALLBACK_SLIDER(echo_depth,reverb,EchoDepth,reverb,modify)
		PRESSED_CALLBACK_SLIDER(modulation_time,reverb,ModulationTime,reverb,modify)
		PRESSED_CALLBACK_SLIDER(modulation_depth,reverb,ModulationDepth,reverb,modify)
		PRESSED_CALLBACK_SLIDER(decay_time,reverb,DecayTime,reverb,modify)
		PRESSED_CALLBACK_SLIDER(decay_lf_ratio,reverb,DecayLFRatio,reverb,modify)
		PRESSED_CALLBACK_SLIDER(decay_hf_ratio,reverb,DecayHFRatio,reverb,modify)
		PRESSED_CALLBACK_SLIDER(reflection_gain,reverb,ReflectionGain,reverb,modify)
		PRESSED_CALLBACK_SLIDER(reflection_delay,reverb,ReflectionDelay,reverb,modify)
		PRESSED_CALLBACK_SLIDER(late_reverb_gain,reverb,LateReverbGain,reverb,modify)
		PRESSED_CALLBACK_SLIDER(late_reverb_delay,reverb,LateReverbDelay,reverb,modify)
		
		/*
		 */
		void reverb_mask_changed(int mask) {
			reverb.setReverbMask(mask);
			reverb_mask_el.setText(editorFormatMask(reverb.getReverbMask()));
		}
		
		/*
		 */
		void preset_clicked() {
			
			float preset[0];
			get_preset(preset_cb.getCurrentItemText(),preset);
			
			if(preset.size()) {
				
				NodesState_ s = new NodesState_();
				s.init_modify(nodes,reverb);
				
				reverb.setGain(preset[0]);
				reverb.setGainLF(preset[1]);
				reverb.setGainHF(preset[2]);
				reverb.setRoomRolloff(preset[3]);
				reverb.setAirAbsorption(preset[4]);
				reverb.setDensity(preset[5]);
				reverb.setDiffusion(preset[6]);
				reverb.setEchoTime(preset[7]);
				reverb.setEchoDepth(preset[8]);
				reverb.setModulationTime(preset[9]);
				reverb.setModulationDepth(preset[10]);
				reverb.setDecayTime(preset[11]);
				reverb.setDecayLFRatio(preset[12]);
				reverb.setDecayHFRatio(preset[13]);
				reverb.setReflectionGain(preset[14]);
				reverb.setReflectionDelay(preset[15]);
				reverb.setLateReverbGain(preset[16]);
				reverb.setLateReverbDelay(preset[17]);
				
				update();
				
				save_state(s);
			}
		}
		
	} /* namespace Reverb */
	
} /* namespace Sound */
