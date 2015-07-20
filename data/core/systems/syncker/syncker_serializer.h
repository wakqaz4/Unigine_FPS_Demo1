/* Copyright (C) 2005-2015, UNIGINE Corp. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.0-RC SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE Corp. at http://unigine.com/
 */


#ifndef __UNIGINE_SYNCKER_SERIALIZER_H__
#define __UNIGINE_SYNCKER_SERIALIZER_H__

namespace Unigine::Syncker {
	
	/**************************************************************************\
	*
	* Serializer
	*
	\**************************************************************************/
	
	/*
	 */
	namespace Serializer {
		
		/**********************************************************************\
		*
		* Frame serialization
		*
		\**********************************************************************/
		
		/*
		 */
		void saveFrame(Stream stream) {
			stream.writeChar(engine.game.isEnabled());
			stream.writeInt(engine.game.getFrame());
			stream.writeFloat(engine.game.getFTime() / engine.game.getScale());
			stream.writeFloat(engine.game.getScale());
			stream.writeFloat(engine.game.getTime());
			stream.writeInt(engine.game.getSeed());
		}
		
		/*
		 */
		void restoreFrame(Stream stream) {
			engine.game.setEnabled(stream.readChar());
			engine.game.setFrame(stream.readInt());
			engine.game.setFTime(stream.readFloat());
			engine.game.setScale(stream.readFloat());
			engine.game.setTime(stream.readFloat());
			engine.game.setSeed(stream.readInt());
		}
		
		/**********************************************************************\
		*
		* Player serialization
		*
		\**********************************************************************/
		
		/*
		 */
		void savePlayer(Player player,Stream stream) {
			stream.writeInt(player.getViewportMask());
			stream.writeInt(player.getReflectionMask());
			stream.writeFloat(player.getFov());
			stream.writeFloat(player.getZNear());
			stream.writeFloat(player.getZFar());
			stream.writeMat4(player.getProjection());
			stream.writeVariable(player.getWorldTransform());
			stream.writeVariable(player.getModelview());
			stream.writeString(player.getPostMaterials());
		}
		
		/*
		 */
		void restorePlayer(Player player,Stream stream) {
			player.setViewportMask(stream.readInt());
			player.setReflectionMask(stream.readInt());
			player.setFov(stream.readFloat());
			player.setZNear(stream.readFloat());
			player.setZFar(stream.readFloat());
			player.setProjection(stream.readMat4());
			player.setWorldTransform(stream.readVariable());
			player.setModelview(stream.readVariable());
			player.setPostMaterials(stream.readString());
		}
		
		/**********************************************************************\
		*
		* Render serialization
		*
		\**********************************************************************/
		
		/*
		 */
		void saveRender(Stream stream) {
			
			// camera parameters
			stream.writeVariable(engine.render.getCameraOffset());
			stream.writeFloat(engine.render.getCameraThreshold());
			
			// distance parameters
			stream.writeFloat(engine.render.getDistanceScale());
			stream.writeFloat(engine.render.getFieldDistance());
			stream.writeFloat(engine.render.getLightDistance());
			stream.writeFloat(engine.render.getDecalDistance());
			stream.writeFloat(engine.render.getObjectDistance());
			stream.writeFloat(engine.render.getClutterDistance());
			stream.writeFloat(engine.render.getShadowDistance());
			stream.writeFloat(engine.render.getReflectionDistance());
			
			// stereo parameters
			stream.writeFloat(engine.render.getStereoDistance());
			stream.writeFloat(engine.render.getStereoRadius());
			stream.writeFloat(engine.render.getStereoOffset());
			
			// tessellation parameters
			stream.writeFloat(engine.render.getTessellationScale());
			stream.writeFloat(engine.render.getTessellationFactor());
			stream.writeFloat(engine.render.getTessellationDistance());
			
			// animation parameters
			stream.writeFloat(engine.render.getAnimationStem());
			stream.writeFloat(engine.render.getAnimationLeaf());
			stream.writeFloat(engine.render.getAnimationScale());
			stream.writeVec3(engine.render.getAnimationWind());
			
			// rendering colors
			stream.writeVec4(engine.render.getAmbientColor());
			stream.writeVec4(engine.render.getReflectionColor());
			stream.writeVec4(engine.render.getBackgroundColor());
			
			// ambient environment
			vec3 coefficients[9];
			engine.render.getEnvironmentCoefficients(coefficients);
			forloop(int i = 0; 9) {
				stream.writeVec3(coefficients[i]);
			}
			stream.writeFloat(engine.render.getEnvironmentExposure());
			
			// light prob environment
			stream.writeInt(engine.render.getLightProbMask());
			stream.writeFloat(engine.render.getLightProbDistance());
			stream.writeFloat(engine.render.getLightProbAttenuation());
			stream.writeFloat(engine.render.getLightProbFresnelBias());
			stream.writeFloat(engine.render.getLightProbFresnelPower());
			stream.writeFloat(engine.render.getLightProbDiffuseScale());
			stream.writeFloat(engine.render.getLightProbSpecularScale());
			
			// ambient occlusion
			stream.writeFloat(engine.render.getOcclusionAmbientScreenRadius());
			stream.writeFloat(engine.render.getOcclusionAmbientSampleRadius());
			stream.writeFloat(engine.render.getOcclusionAmbientPerspective());
			stream.writeFloat(engine.render.getOcclusionAmbientAttenuation());
			stream.writeFloat(engine.render.getOcclusionAmbientThreshold());
			stream.writeFloat(engine.render.getOcclusionAmbientEmitter());
			stream.writeFloat(engine.render.getOcclusionAmbientReceiver());
			stream.writeFloat(engine.render.getOcclusionAmbientScale());
			stream.writeFloat(engine.render.getOcclusionAmbientPower());
			
			// light occlusion
			stream.writeFloat(engine.render.getOcclusionLightScreenRadius());
			stream.writeFloat(engine.render.getOcclusionLightSampleRadius());
			stream.writeFloat(engine.render.getOcclusionLightPerspective());
			stream.writeFloat(engine.render.getOcclusionLightAttenuation());
			stream.writeFloat(engine.render.getOcclusionLightThreshold());
			stream.writeFloat(engine.render.getOcclusionLightEmitter());
			stream.writeFloat(engine.render.getOcclusionLightReceiver());
			stream.writeFloat(engine.render.getOcclusionLightScale());
			stream.writeFloat(engine.render.getOcclusionLightPower());
			
			// refraction
			stream.writeVec3(engine.render.getRefractionDispersion());
			
			// motion blur
			stream.writeFloat(engine.render.getMotionBlurLinearScale());
			stream.writeFloat(engine.render.getMotionBlurAngularScale());
			stream.writeFloat(engine.render.getMotionBlurVelocityScale());
			stream.writeFloat(engine.render.getMotionBlurMaxVelocity());
			
			// light scattering
			stream.writeVec4(engine.render.getScatteringSunColor());
			stream.writeVec4(engine.render.getScatteringRayColor());
			stream.writeVec4(engine.render.getScatteringMieColor());
			stream.writeFloat(engine.render.getScatteringInnerAngle());
			stream.writeFloat(engine.render.getScatteringOuterAngle());
			stream.writeFloat(engine.render.getScatteringRefractive());
			stream.writeFloat(engine.render.getScatteringDensity());
			stream.writeFloat(engine.render.getScatteringDepolarization());
			stream.writeFloat(engine.render.getScatteringTurbidity());
			stream.writeVec3(engine.render.getScatteringDirection());
			stream.writeFloat(engine.render.getScatteringGround());
			stream.writeFloat(engine.render.getScatteringAreal());
			stream.writeFloat(engine.render.getScatteringRayHeight());
			stream.writeFloat(engine.render.getScatteringMieHeight());
			stream.writeFloat(engine.render.getScatteringGreenstein());
			stream.writeFloat(engine.render.getScatteringAngularity());
			stream.writeFloat(engine.render.getScatteringThreshold());
			stream.writeFloat(engine.render.getScatteringEnergy());
			stream.writeFloat(engine.render.getScatteringPower());
			stream.writeFloat(engine.render.getScatteringSun());
			stream.writeFloat(engine.render.getScatteringRay());
			stream.writeFloat(engine.render.getScatteringMie());
			
			// volumetric scattering
			stream.writeFloat(engine.render.getVolumetricExposure());
			stream.writeFloat(engine.render.getVolumetricLength());
			stream.writeFloat(engine.render.getVolumetricAttenuation());
			
			// glow post-processing
			stream.writeFloat(engine.render.getGlowThreshold());
			stream.writeFloat(engine.render.getGlowSmallExposure());
			stream.writeFloat(engine.render.getGlowMediumExposure());
			stream.writeFloat(engine.render.getGlowLargeExposure());
			
			// depth of field post-processing
			stream.writeFloat(engine.render.getDOFFocalDistance());
			stream.writeFloat(engine.render.getDOFFarBlurRange());
			stream.writeFloat(engine.render.getDOFFarBlurRadius());
			stream.writeFloat(engine.render.getDOFFarBlurPower());
			stream.writeFloat(engine.render.getDOFFarFocalRange());
			stream.writeFloat(engine.render.getDOFFarFocalScale());
			stream.writeFloat(engine.render.getDOFFarFocalPower());
			stream.writeFloat(engine.render.getDOFNearBlurRange());
			stream.writeFloat(engine.render.getDOFNearBlurRadius());
			stream.writeFloat(engine.render.getDOFNearBlurPower());
			stream.writeFloat(engine.render.getDOFNearFocalRange());
			stream.writeFloat(engine.render.getDOFNearFocalScale());
			stream.writeFloat(engine.render.getDOFNearFocalPower());
			
			// hdr post-processing
			stream.writeFloat(engine.render.getHDRExposure());
			stream.writeFloat(engine.render.getHDRAdaptation());
			stream.writeFloat(engine.render.getHDRMinLuminance());
			stream.writeFloat(engine.render.getHDRMaxLuminance());
			stream.writeFloat(engine.render.getHDRThreshold());
			stream.writeFloat(engine.render.getHDRSmallExposure());
			stream.writeFloat(engine.render.getHDRMediumExposure());
			stream.writeFloat(engine.render.getHDRLargeExposure());
			stream.writeFloat(engine.render.getHDRBrightExposure());
			
			// hdr filmic post processing
			stream.writeFloat(engine.render.getHDRFilmicShoulderScale());
			stream.writeFloat(engine.render.getHDRFilmicLinearScale());
			stream.writeFloat(engine.render.getHDRFilmicLinearAngle());
			stream.writeFloat(engine.render.getHDRFilmicToeScale());
			stream.writeFloat(engine.render.getHDRFilmicToeNumerator());
			stream.writeFloat(engine.render.getHDRFilmicToeDenominator());
			stream.writeFloat(engine.render.getHDRFilmicWhiteLevel());
			
			// hdr cross post processing
			stream.writeChar(engine.render.isHDRCrossEnabled());
			stream.writeVec4(engine.render.getHDRCrossColor());
			stream.writeFloat(engine.render.getHDRCrossScale());
			stream.writeChar(engine.render.getHDRCrossShafts());
			stream.writeFloat(engine.render.getHDRCrossLength());
			stream.writeFloat(engine.render.getHDRCrossAngle());
			stream.writeFloat(engine.render.getHDRCrossThreshold());
			
			// hdr bokeh post processing
			stream.writeChar(engine.render.isHDRBokehEnabled());
			stream.writeVec4(engine.render.getHDRBokehColor());
			stream.writeFloat(engine.render.getHDRBokehScale());
			stream.writeFloat(engine.render.getHDRBokehFarSize());
			stream.writeFloat(engine.render.getHDRBokehFarPower());
			stream.writeFloat(engine.render.getHDRBokehNearSize());
			stream.writeFloat(engine.render.getHDRBokehNearPower());
			stream.writeFloat(engine.render.getHDRBokehThreshold());
			stream.writeString(engine.render.getHDRBokehTextureName());
			
			// hdr shaft post processing
			stream.writeChar(engine.render.isHDRShaftEnabled());
			stream.writeVec4(engine.render.getHDRShaftColor());
			stream.writeFloat(engine.render.getHDRShaftScale());
			stream.writeFloat(engine.render.getHDRShaftLength());
			stream.writeFloat(engine.render.getHDRShaftAttenuation());
			stream.writeFloat(engine.render.getHDRShaftThreshold());
			
			// hdr lens post processing
			stream.writeChar(engine.render.isHDRLensEnabled());
			stream.writeVec4(engine.render.getHDRLensColor());
			stream.writeFloat(engine.render.getHDRLensScale());
			stream.writeFloat(engine.render.getHDRLensLength());
			stream.writeFloat(engine.render.getHDRLensRadius());
			stream.writeFloat(engine.render.getHDRLensThreshold());
			stream.writeVec3(engine.render.getHDRLensDispersion());
			stream.writeString(engine.render.getHDRLensTextureName());
			
			// color post-processing
			stream.writeFloat(engine.render.getColorBrightness());
			stream.writeFloat(engine.render.getColorContrast());
			stream.writeFloat(engine.render.getColorSaturation());
			stream.writeFloat(engine.render.getColorGamma());
			stream.writeVec4(engine.render.getColorWhite());
			
			// render materials
			stream.writeString(engine.render.getRenderMaterials());
			
			// composite material
			stream.writeString(engine.render.getCompositeMaterial());
			
			// post materials
			stream.writeString(engine.render.getPostMaterials());
			
			// fade post-processing
			stream.writeVec4(engine.render.getFadeColor());
		}
		
		/*
		 */
		void restoreRender(Stream stream) {
			
			// camera parameters
			engine.render.setCameraOffset(stream.readVariable());
			engine.render.setCameraThreshold(stream.readFloat());
			
			// distance parameters
			engine.render.setDistanceScale(stream.readFloat());
			engine.render.setFieldDistance(stream.readFloat());
			engine.render.setLightDistance(stream.readFloat());
			engine.render.setDecalDistance(stream.readFloat());
			engine.render.setObjectDistance(stream.readFloat());
			engine.render.setClutterDistance(stream.readFloat());
			engine.render.setShadowDistance(stream.readFloat());
			engine.render.setReflectionDistance(stream.readFloat());
			
			// stereo parameters
			engine.render.setStereoDistance(stream.readFloat());
			engine.render.setStereoRadius(stream.readFloat());
			engine.render.setStereoOffset(stream.readFloat());
			
			// tessellation parameters
			engine.render.setTessellationScale(stream.readFloat());
			engine.render.setTessellationFactor(stream.readFloat());
			engine.render.setTessellationDistance(stream.readFloat());
			
			// animation parameters
			engine.render.setAnimationStem(stream.readFloat());
			engine.render.setAnimationLeaf(stream.readFloat());
			engine.render.setAnimationScale(stream.readFloat());
			engine.render.setAnimationWind(stream.readVec3());
			
			// rendering colors
			engine.render.setAmbientColor(stream.readVec4());
			engine.render.setReflectionColor(stream.readVec4());
			engine.render.setBackgroundColor(stream.readVec4());
			
			// ambient environment
			vec3 coefficients[9];
			forloop(int i = 0; 9) {
				coefficients[i] = stream.readVec3();
			}
			engine.render.setEnvironmentCoefficients(coefficients);
			engine.render.setEnvironmentExposure(stream.readFloat());
			
			// light prob environment
			engine.render.setLightProbMask(stream.readInt());
			engine.render.setLightProbDistance(stream.readFloat());
			engine.render.setLightProbAttenuation(stream.readFloat());
			engine.render.setLightProbFresnelBias(stream.readFloat());
			engine.render.setLightProbFresnelPower(stream.readFloat());
			engine.render.setLightProbDiffuseScale(stream.readFloat());
			engine.render.setLightProbSpecularScale(stream.readFloat());
			
			// ambient occlusion
			engine.render.setOcclusionAmbientScreenRadius(stream.readFloat());
			engine.render.setOcclusionAmbientSampleRadius(stream.readFloat());
			engine.render.setOcclusionAmbientPerspective(stream.readFloat());
			engine.render.setOcclusionAmbientAttenuation(stream.readFloat());
			engine.render.setOcclusionAmbientThreshold(stream.readFloat());
			engine.render.setOcclusionAmbientEmitter(stream.readFloat());
			engine.render.setOcclusionAmbientReceiver(stream.readFloat());
			engine.render.setOcclusionAmbientScale(stream.readFloat());
			engine.render.setOcclusionAmbientPower(stream.readFloat());
			
			// light occlusion
			engine.render.setOcclusionLightScreenRadius(stream.readFloat());
			engine.render.setOcclusionLightSampleRadius(stream.readFloat());
			engine.render.setOcclusionLightPerspective(stream.readFloat());
			engine.render.setOcclusionLightAttenuation(stream.readFloat());
			engine.render.setOcclusionLightThreshold(stream.readFloat());
			engine.render.setOcclusionLightEmitter(stream.readFloat());
			engine.render.setOcclusionLightReceiver(stream.readFloat());
			engine.render.setOcclusionLightScale(stream.readFloat());
			engine.render.setOcclusionLightPower(stream.readFloat());
			
			// refraction
			engine.render.setRefractionDispersion(stream.readVec3());
			
			// motion blur
			engine.render.setMotionBlurLinearScale(stream.readFloat());
			engine.render.setMotionBlurAngularScale(stream.readFloat());
			engine.render.setMotionBlurVelocityScale(stream.readFloat());
			engine.render.setMotionBlurMaxVelocity(stream.readFloat());
			
			// light scattering
			engine.render.setScatteringSunColor(stream.readVec4());
			engine.render.setScatteringRayColor(stream.readVec4());
			engine.render.setScatteringMieColor(stream.readVec4());
			engine.render.setScatteringInnerAngle(stream.readFloat());
			engine.render.setScatteringOuterAngle(stream.readFloat());
			engine.render.setScatteringRefractive(stream.readFloat());
			engine.render.setScatteringDensity(stream.readFloat());
			engine.render.setScatteringDepolarization(stream.readFloat());
			engine.render.setScatteringTurbidity(stream.readFloat());
			engine.render.setScatteringDirection(stream.readVec3());
			engine.render.setScatteringGround(stream.readFloat());
			engine.render.setScatteringAreal(stream.readFloat());
			engine.render.setScatteringRayHeight(stream.readFloat());
			engine.render.setScatteringMieHeight(stream.readFloat());
			engine.render.setScatteringGreenstein(stream.readFloat());
			engine.render.setScatteringAngularity(stream.readFloat());
			engine.render.setScatteringThreshold(stream.readFloat());
			engine.render.setScatteringEnergy(stream.readFloat());
			engine.render.setScatteringPower(stream.readFloat());
			engine.render.setScatteringSun(stream.readFloat());
			engine.render.setScatteringRay(stream.readFloat());
			engine.render.setScatteringMie(stream.readFloat());
			
			// volumetric scattering
			engine.render.setVolumetricExposure(stream.readFloat());
			engine.render.setVolumetricLength(stream.readFloat());
			engine.render.setVolumetricAttenuation(stream.readFloat());
			
			// glow post-processing
			engine.render.setGlowThreshold(stream.readFloat());
			engine.render.setGlowSmallExposure(stream.readFloat());
			engine.render.setGlowMediumExposure(stream.readFloat());
			engine.render.setGlowLargeExposure(stream.readFloat());
			
			// depth of field post-processing
			engine.render.setDOFFocalDistance(stream.readFloat());
			engine.render.setDOFFarBlurRange(stream.readFloat());
			engine.render.setDOFFarBlurRadius(stream.readFloat());
			engine.render.setDOFFarBlurPower(stream.readFloat());
			engine.render.setDOFFarFocalRange(stream.readFloat());
			engine.render.setDOFFarFocalScale(stream.readFloat());
			engine.render.setDOFFarFocalPower(stream.readFloat());
			engine.render.setDOFNearBlurRange(stream.readFloat());
			engine.render.setDOFNearBlurRadius(stream.readFloat());
			engine.render.setDOFNearBlurPower(stream.readFloat());
			engine.render.setDOFNearFocalRange(stream.readFloat());
			engine.render.setDOFNearFocalScale(stream.readFloat());
			engine.render.setDOFNearFocalPower(stream.readFloat());
			
			// hdr post-processing
			engine.render.setHDRExposure(stream.readFloat());
			engine.render.setHDRAdaptation(stream.readFloat());
			engine.render.setHDRMinLuminance(stream.readFloat());
			engine.render.setHDRMaxLuminance(stream.readFloat());
			engine.render.setHDRThreshold(stream.readFloat());
			engine.render.setHDRSmallExposure(stream.readFloat());
			engine.render.setHDRMediumExposure(stream.readFloat());
			engine.render.setHDRLargeExposure(stream.readFloat());
			engine.render.setHDRBrightExposure(stream.readFloat());
			
			// hdr filmic post processing
			engine.render.setHDRFilmicShoulderScale(stream.readFloat());
			engine.render.setHDRFilmicLinearScale(stream.readFloat());
			engine.render.setHDRFilmicLinearAngle(stream.readFloat());
			engine.render.setHDRFilmicToeScale(stream.readFloat());
			engine.render.setHDRFilmicToeNumerator(stream.readFloat());
			engine.render.setHDRFilmicToeDenominator(stream.readFloat());
			engine.render.setHDRFilmicWhiteLevel(stream.readFloat());
			
			// hdr cross post processing
			engine.render.setHDRCrossEnabled(stream.readChar());
			engine.render.setHDRCrossColor(stream.readVec4());
			engine.render.setHDRCrossScale(stream.readFloat());
			engine.render.setHDRCrossShafts(stream.readChar());
			engine.render.setHDRCrossLength(stream.readFloat());
			engine.render.setHDRCrossAngle(stream.readFloat());
			engine.render.setHDRCrossThreshold(stream.readFloat());
			
			// hdr bokeh post processing
			engine.render.setHDRBokehEnabled(stream.readChar());
			engine.render.setHDRBokehColor(stream.readVec4());
			engine.render.setHDRBokehScale(stream.readFloat());
			engine.render.setHDRBokehFarSize(stream.readFloat());
			engine.render.setHDRBokehFarPower(stream.readFloat());
			engine.render.setHDRBokehNearSize(stream.readFloat());
			engine.render.setHDRBokehNearPower(stream.readFloat());
			engine.render.setHDRBokehThreshold(stream.readFloat());
			engine.render.setHDRBokehTextureName(stream.readString());
			
			// hdr shaft post processing
			engine.render.setHDRShaftEnabled(stream.readChar());
			engine.render.setHDRShaftColor(stream.readVec4());
			engine.render.setHDRShaftScale(stream.readFloat());
			engine.render.setHDRShaftLength(stream.readFloat());
			engine.render.setHDRShaftAttenuation(stream.readFloat());
			engine.render.setHDRShaftThreshold(stream.readFloat());
			
			// hdr lens post processing
			engine.render.setHDRLensEnabled(stream.readChar());
			engine.render.setHDRLensColor(stream.readVec4());
			engine.render.setHDRLensScale(stream.readFloat());
			engine.render.setHDRLensLength(stream.readFloat());
			engine.render.setHDRLensRadius(stream.readFloat());
			engine.render.setHDRLensThreshold(stream.readFloat());
			engine.render.setHDRLensDispersion(stream.readVec3());
			engine.render.setHDRLensTextureName(stream.readString());
			
			// color post-processing
			engine.render.setColorBrightness(stream.readFloat());
			engine.render.setColorContrast(stream.readFloat());
			engine.render.setColorSaturation(stream.readFloat());
			engine.render.setColorGamma(stream.readFloat());
			engine.render.setColorWhite(stream.readVec4());
			
			// render materials
			engine.render.setRenderMaterials(stream.readString());
			
			// composite material
			engine.render.setCompositeMaterial(stream.readString());
			
			// post materials
			engine.render.setPostMaterials(stream.readString());
			
			// fade post-processing
			engine.render.setFadeColor(stream.readVec4());
		}
		
		/**********************************************************************\
		*
		* Material serialization
		*
		\**********************************************************************/
		
		/*
		 */
		void saveMaterial(Material material,Stream stream) {
			
			// blending
			stream.writeChar(material.getBlendSrcFunc());
			stream.writeChar(material.getBlendDestFunc());
			
			// options
			stream.writeChar(material.getDownsize());
			stream.writeShort(material.getOrder());
			stream.writeInt(material.getOffset());
			stream.writeInt(material.getLightMask());
			stream.writeInt(material.getViewportMask());
			
			// material flags
			int flags = 0;
			if(material.getPostDeferred()) flags |= 1 << 0;
			if(material.getPostRefraction()) flags |= 1 << 1;
			if(material.getPostScattering()) flags |= 1 << 2;
			if(material.getDepthMask()) flags |= 1 << 3;
			if(material.getGlowMask()) flags |= 1 << 4;
			if(material.getParallaxMask()) flags |= 1 << 5;
			if(material.getAlphaTest()) flags |= 1 << 6;
			if(material.getTwoSided()) flags |= 1 << 7;
			if(material.getCastShadow()) flags |= 1 << 8;
			if(material.getReceiveShadow()) flags |= 1 << 9;
			if(material.getCastWorldShadow()) flags |= 1 << 10;
			if(material.getReceiveWorldShadow()) flags |= 1 << 11;
			if(material.getCastTranslucent()) flags |= 1 << 12;
			if(material.getReceiveTranslucent()) flags |= 1 << 13;
			stream.writeShort(flags);
			
			// states
			stream.writeChar(material.getNumStates());
			forloop(int i = 0; material.getNumStates()) {
				stream.writeString(material.getStateName(i));
				stream.writeChar(material.getState(i));
			}
			
			// textures
			int num_textures = 0;
			forloop(int i = 0; material.getNumTextures()) {
				if(material.getTextureType(i) != MATERIAL_TEXTURE_IMAGE) continue;
				num_textures++;
			}
			stream.writeChar(num_textures);
			forloop(int i = 0; material.getNumTextures()) {
				if(material.getTextureType(i) != MATERIAL_TEXTURE_IMAGE) continue;
				stream.writeString(material.getTextureName(i));
				stream.writeString(material.getImageTextureName(i));
			}
			
			// parameters
			stream.writeChar(material.getNumParameters());
			forloop(int i = 0; material.getNumParameters()) {
				stream.writeString(material.getParameterName(i));
				stream.writeVec4(material.getParameter(i));
			}
		}
		
		/*
		 */
		void restoreMaterial(Material material,Stream stream) {
			
			// blending
			int src_func = stream.readChar();
			int dest_func = stream.readChar();
			material.setBlendFunc(src_func,dest_func);
			
			// options
			material.setDownsize(stream.readChar());
			material.setOrder(stream.readShort());
			material.setOffset(stream.readInt());
			material.setLightMask(stream.readInt());
			material.setViewportMask(stream.readInt());
			
			// material flags
			int flags = stream.readShort();
			material.setPostDeferred((flags & (1 << 0)) != 0);
			material.setPostRefraction((flags & (1 << 1)) != 0);
			material.setPostScattering((flags & (1 << 2)) != 0);
			material.setDepthMask((flags & (1 << 3)) != 0);
			material.setGlowMask((flags & (1 << 4)) != 0);
			material.setParallaxMask((flags & (1 << 5)) != 0);
			material.setAlphaTest((flags & (1 << 6)) != 0);
			material.setTwoSided((flags & (1 << 7)) != 0);
			material.setCastShadow((flags & (1 << 8)) != 0);
			material.setReceiveShadow((flags & (1 << 9)) != 0);
			material.setCastWorldShadow((flags & (1 << 10)) != 0);
			material.setReceiveWorldShadow((flags & (1 << 11)) != 0);
			material.setCastTranslucent((flags & (1 << 12)) != 0);
			material.setReceiveTranslucent((flags & (1 << 13)) != 0);
			
			// states
			forloop(int i = 0; stream.readChar()) {
				int state = material.findState(stream.readString(),MATERIAL_STATE_AUX + 32 + i);
				int value = stream.readChar();
				if(state != -1 && material.getState(state) != value) material.setState(state,value);
			}
			
			// textures
			forloop(int i = 0; stream.readChar()) {
				int texture = material.findTexture(stream.readString(),MATERIAL_TEXTURE_AUX + 32 + i);
				string name = stream.readString();
				if(texture != -1 && material.getImageTextureName(texture) != name) material.setImageTextureName(texture,name);
			}
			
			// parameters
			forloop(int i = 0; stream.readChar()) {
				int parameter = material.findParameter(stream.readString(),MATERIAL_PARAMETER_AUX + 32 + i);
				vec4 value = stream.readVec4();
				if(parameter != -1 && material.getParameter(parameter) != value) material.setParameter(parameter,value);
			}
		}
		
		/**********************************************************************\
		*
		* Node serialization
		*
		\**********************************************************************/
		
		/*
		 */
		void sendNode(Node node,Stream stream) {
			
			node = class_cast(node.getTypeName(),node);
			
			// node type
			stream.writeChar(node.getType());
			
			// save node
			switch(node.getType()) {
				
				// node reference
				case NODE_REFERENCE: {
					NodeReference reference = node;
					stream.writeString(reference.getNodeName());
					break;
				}
				
				// lights
				case NODE_LIGHT_OMNI: {
					LightOmni omni = node;
					stream.writeString(omni.getImageTextureName());
					break;
				}
				case NODE_LIGHT_PROJ: {
					LightProj proj = node;
					stream.writeString(proj.getImageTextureName());
					break;
				}
				
				// objects
				case NODE_OBJECT_MESH_STATIC: {
					ObjectMeshStatic mesh = node;
					stream.writeString(mesh.getMeshName());
					break;
				}
				case NODE_OBJECT_MESH_SKINNED: {
					ObjectMeshSkinned mesh = node;
					stream.writeString(mesh.getMeshName());
					break;
				}
			}
			
			// type parameters
			if(node.isLight()) send_light(node,stream);
			else if(node.isObject()) send_object(node,stream);
			
			// save node
			saveNode(node,stream);
		}
		
		/*
		 */
		Node receiveNode(Stream stream) {
			
			Node node = NULL;
			
			// node type
			int type = stream.readChar();
			
			// create node
			switch(type) {
				
				// node reference
				case NODE_REFERENCE:	node = new NodeReference(stream.readString()); break;
				
				// lights
				case NODE_LIGHT_PROB:	node = new LightProb(vec4_one,vec3_one); break;
				case NODE_LIGHT_SPOT:	node = new LightSpot(vec4_one,1.0f); break;
				case NODE_LIGHT_OMNI:	node = new LightOmni(vec4_one,vec3_one,stream.readString()); break;
				case NODE_LIGHT_PROJ:	node = new LightProj(vec4_one,1.0f,60.0f,stream.readString()); break;
				case NODE_LIGHT_WORLD:	node = new LightWorld(vec4_one); break;
				
				// objects
				case NODE_OBJECT_MESH_STATIC:	node = new ObjectMeshStatic(stream.readString()); break;
				case NODE_OBJECT_MESH_SKINNED:	node = new ObjectMeshSkinned(stream.readString()); break;
				case NODE_OBJECT_PARTICLES:		node = new ObjectParticles(); break;
				case NODE_OBJECT_BILLBOARD:		node = new ObjectBillboard(1.0f,1.0f); break;
				case NODE_OBJECT_VOLUME_SPHERE:	node = new ObjectVolumeSphere(vec3_one); break;
				case NODE_OBJECT_VOLUME_BOX:	node = new ObjectVolumeBox(vec3_one); break;
				case NODE_OBJECT_VOLUME_OMNI:	node = new ObjectVolumeOmni(1.0f,1.0f,1.0f); break;
				case NODE_OBJECT_VOLUME_PROJ:	node = new ObjectVolumeProj(0.01f,1.0f,60.0f); break;
				case NODE_OBJECT_WATER:			node = new ObjectWater(); break;
				case NODE_OBJECT_SKY:			node = new ObjectSky(); break;
				
				default:				throw("Unigine::Syncker::Serializer::receiveNode(): unknown node type \"%s\"\n",node.getTypeName());
			}
			
			// type parameters
			if(node.isLight()) receive_light(node,stream);
			else if(node.isObject()) receive_object(node,stream);
			
			// restore node
			restoreNode(node,stream);
			
			return node;
		}
		
		/*
		 */
		void saveNode(Node node,Stream stream) {
			
			node = class_cast(node.getTypeName(),node);
			
			// type parameters
			if(node.isLight()) save_light(node,stream);
			else if(node.isObject()) save_object(node,stream);
			
			// node flags
			int flags = 0;
			if(node.isEnabled()) flags |= 1 << 0;
			if(node.isCollider()) flags |= 1 << 1;
			if(node.isClutter()) flags |= 1 << 2;
			if(node.isSpatial()) flags |= 1 << 3;
			if(node.isQuery()) flags |= 1 << 4;
			stream.writeChar(flags);
			
			// transformation
			#ifdef USE_DOUBLE
				stream.writeDMat4(node.getWorldTransform());
			#else
				stream.writeMat44(node.getWorldTransform());
			#endif
		}
		
		/*
		 */
		void restoreNode(Node node,Stream stream) {
			
			node = class_cast(node.getTypeName(),node);
			
			// type parameters
			if(node.isLight()) restore_light(node,stream);
			else if(node.isObject()) restore_object(node,stream);
			
			// node flags
			int flags = stream.readChar();
			node.setEnabled((flags & (1 << 0)) != 0);
			node.setCollider((flags & (1 << 1)) != 0);
			node.setClutter((flags & (1 << 2)) != 0);
			node.setSpatial((flags & (1 << 3)) != 0);
			node.setQuery((flags & (1 << 4)) != 0);
			
			// transformation
			#ifdef USE_DOUBLE
				node.setWorldTransform(stream.readDMat4());
			#else
				node.setWorldTransform(stream.readMat44());
			#endif
		}
		
		/**********************************************************************\
		*
		* Light serialization
		*
		\**********************************************************************/
		
		/*
		 */
		void send_light(Light light,Stream stream) {
			
			// light masks
			stream.writeInt(light.getLightMask());
			stream.writeInt(light.getViewportMask());
			
			// light flags
			stream.writeChar(light.isDeferred());
			
			// shadow parameters
			stream.writeChar(light.getShadow());
			stream.writeChar(light.getTranslucent());
			stream.writeChar(light.getShadowSize());
			stream.writeChar(light.getTranslucentSize());
			stream.writeVec3(light.getShadowOffset());
			stream.writeFloat(light.getShadowBias());
			stream.writeFloat(light.getShadowSlope());
			stream.writeFloat(light.getShadowSoftness());
			stream.writeFloat(light.getTranslucentBias());
			
			// level of detail
			stream.writeFloat(light.getDeferredDistance());
			stream.writeFloat(light.getVisibleDistance());
			stream.writeFloat(light.getShadowDistance());
			stream.writeFloat(light.getFadeDistance());
			
			// type parameters
			switch(light.getType()) {
				
				// light omni
				case NODE_LIGHT_OMNI: {
					LightOmni omni = light;
					stream.writeChar(omni.getShadowMask());
					break;
				}
				
				// light world
				case NODE_LIGHT_WORLD: {
					LightWorld world = light;
					stream.writeChar(world.getNumShadowSplits());
					break;
				}
			}
		}
		
		/*
		 */
		void receive_light(Light light,Stream stream) {
			
			// light masks
			light.setLightMask(stream.readInt());
			light.setViewportMask(stream.readInt());
			
			// light flags
			light.setDeferred(stream.readChar());
			
			// shadow parameters
			light.setShadow(stream.readChar());
			light.setTranslucent(stream.readChar());
			light.setShadowSize(stream.readChar());
			light.setTranslucentSize(stream.readChar());
			light.setShadowOffset(stream.readVec3());
			light.setShadowBias(stream.readFloat());
			light.setShadowSlope(stream.readFloat());
			light.setShadowSoftness(stream.readFloat());
			light.setTranslucentBias(stream.readFloat());
			
			// level of detail
			light.setDeferredDistance(stream.readFloat());
			light.setVisibleDistance(stream.readFloat());
			light.setShadowDistance(stream.readFloat());
			light.setFadeDistance(stream.readFloat());
			
			// type parameters
			switch(light.getType()) {
				
				// light omni
				case NODE_LIGHT_OMNI: {
					LightOmni omni = light;
					omni.setShadowMask(stream.readChar());
					break;
				}
				
				// light world
				case NODE_LIGHT_WORLD: {
					LightWorld world = light;
					world.setNumShadowSplits(stream.readChar());
					break;
				}
			}
		}
		
		/*
		 */
		void save_light(Light light,Stream stream) {
			
			// common parameters
			stream.writeVec4(light.getColor());
			stream.writeFloat(light.getMultiplier());
			stream.writeFloat(light.getAttenuation());
			stream.writeFloat(light.getDiffuseScale());
			stream.writeFloat(light.getSpecularScale());
			stream.writeFloat(light.getSpecularPower());
			stream.writeFloat(light.getShadowAmbient());
			
			// type parameters
			switch(light.getType()) {
				
				// light prob
				case NODE_LIGHT_PROB: {
					vec3 coefficients[9];
					LightProb prob = light;
					stream.writeVec3(prob.getRadius());
					prob.getCoefficients(coefficients);
					forloop(int i = 0; 9) {
						stream.writeVec3(coefficients[i]);
					}
					break;
				}
				
				// light spot
				case NODE_LIGHT_SPOT: {
					LightSpot spot = light;
					stream.writeFloat(spot.getRadius());
					break;
				}
				
				// light omni
				case NODE_LIGHT_OMNI: {
					LightOmni omni = light;
					stream.writeVec3(omni.getRadius());
					break;
				}
				
				// light proj
				case NODE_LIGHT_PROJ: {
					LightProj proj = light;
					stream.writeFloat(proj.getRadius());
					stream.writeFloat(proj.getFov());
					stream.writeFloat(proj.getZNear());
					break;
				}
				
				// light world
				case NODE_LIGHT_WORLD: {
					LightWorld world = light;
					stream.writeChar(world.getScattering());
					stream.writeFloat(world.getShadowRange());
					stream.writeFloat(world.getShadowDistribute());
					break;
				}
			}
		}
		
		/*
		 */
		void restore_light(Light light,Stream stream) {
			
			// common parameters
			light.setColor(stream.readVec4());
			light.setMultiplier(stream.readFloat());
			light.setAttenuation(stream.readFloat());
			light.setDiffuseScale(stream.readFloat());
			light.setSpecularScale(stream.readFloat());
			light.setSpecularPower(stream.readFloat());
			light.setShadowAmbient(stream.readFloat());
			
			// type parameters
			switch(light.getType()) {
				
				// light prob
				case NODE_LIGHT_PROB: {
					vec3 coefficients[9];
					LightProb prob = light;
					prob.setRadius(stream.readVec3());
					forloop(int i = 0; 9) {
						coefficients[i] = stream.readVec3();
					}
					prob.setCoefficients(coefficients);
					break;
				}
				
				// light spot
				case NODE_LIGHT_SPOT: {
					LightSpot spot = light;
					spot.setRadius(stream.readFloat());
					break;
				}
				
				// light omni
				case NODE_LIGHT_OMNI: {
					LightOmni omni = light;
					omni.setRadius(stream.readVec3());
					break;
				}
				
				// light proj
				case NODE_LIGHT_PROJ: {
					LightProj proj = light;
					proj.setRadius(stream.readFloat());
					proj.setFov(stream.readFloat());
					proj.setZNear(stream.readFloat());
					break;
				}
				
				// light world
				case NODE_LIGHT_WORLD: {
					LightWorld world = light;
					world.setScattering(stream.readChar());
					world.setShadowRange(stream.readFloat());
					world.setShadowDistribute(stream.readFloat());
					break;
				}
			}
		}
		
		/**********************************************************************\
		*
		* Object serialization
		*
		\**********************************************************************/
		
		/*
		 */
		void send_object(Object object,Stream stream) {
			
			// object surfaces
			stream.writeInt(object.getNumSurfaces());
			forloop(int i = 0; object.getNumSurfaces()) {
				
				stream.writeInt(object.getLightMask(i));
				stream.writeInt(object.getViewportMask(i));
				
				stream.writeInt(object.getMinParent(i));
				stream.writeFloat(object.getMinVisibleDistance(i));
				stream.writeFloat(object.getMinFadeDistance(i));
				
				stream.writeInt(object.getMaxParent(i));
				stream.writeFloat(object.getMaxVisibleDistance(i));
				stream.writeFloat(object.getMaxFadeDistance(i));
			}
			
			// type parameters
			switch(object.getType()) {
				
				// object billboard
				case NODE_OBJECT_BILLBOARD: {
					ObjectBillboard billboard = object;
					stream.writeVec3(billboard.getOffset());
					stream.writeChar(billboard.getAxisX());
					stream.writeChar(billboard.getAxisZ());
					stream.writeChar(billboard.getScreen());
					break;
				}
				
				// object sky
				case NODE_OBJECT_SKY: {
					ObjectSky sky = object;
					stream.writeChar(sky.isSpherical());
					stream.writeVec3(sky.getSize());
					stream.writeInt(sky.getMinSlices());
					stream.writeInt(sky.getMaxSlices());
					stream.writeFloat(sky.getDistribute());
					stream.writeString(sky.getDensityImageName());
					break;
				}
			}
		}
		
		/*
		 */
		void receive_object(Object object,Stream stream) {
			
			// object surfaces
			int num_surfaces = object.getNumSurfaces();
			forloop(int i = 0; stream.readInt()) {
				
				int light_mask = stream.readInt();
				int viewport_mask = stream.readInt();
				
				int min_parent = stream.readInt();
				float min_visible_distance = stream.readFloat();
				float min_fade_distance = stream.readFloat();
				
				int max_parent = stream.readInt();
				float max_visible_distance = stream.readFloat();
				float max_fade_distance = stream.readFloat();
				
				if(i < num_surfaces) {
					
					object.setLightMask(light_mask,i);
					object.setViewportMask(viewport_mask,i);
					
					object.setMinParent(min_parent,i);
					object.setMinVisibleDistance(min_visible_distance,i);
					object.setMinFadeDistance(min_fade_distance,i);
					
					object.setMaxParent(max_parent,i);
					object.setMaxVisibleDistance(max_visible_distance,i);
					object.setMaxFadeDistance(max_fade_distance,i);
				}
			}
			
			// type parameters
			switch(object.getType()) {
				
				// object billboard
				case NODE_OBJECT_BILLBOARD: {
					ObjectBillboard billboard = object;
					billboard.setOffset(stream.readVec3());
					billboard.setAxisX(stream.readChar());
					billboard.setAxisZ(stream.readChar());
					billboard.setScreen(stream.readChar());
					break;
				}
				
				// object sky
				case NODE_OBJECT_SKY: {
					ObjectSky sky = object;
					sky.setSpherical(stream.readChar());
					sky.setSize(stream.readVec3());
					sky.setMinSlices(stream.readInt());
					sky.setMaxSlices(stream.readInt());
					sky.setDistribute(stream.readFloat());
					sky.setDensityImageName(stream.readString());
					break;
				}
			}
		}
		
		/*
		 */
		void save_object(Object object,Stream stream) {
			
			// surface parameters
			stream.writeInt(object.getNumSurfaces());
			forloop(int i = 0; object.getNumSurfaces()) {
				int flags = 0;
				if(object.isEnabled(i)) flags |= 1 << 0;
				if(object.getCastShadow(i)) flags |= 1 << 1;
				if(object.getReceiveShadow(i)) flags |= 1 << 2;
				if(object.getCastWorldShadow(i)) flags |= 1 << 3;
				if(object.getReceiveWorldShadow(i)) flags |= 1 << 4;
				stream.writeChar(flags);
				stream.writeString(object.getMaterialName(i));
			}
			
			// type parameters
			switch(object.getType()) {
				
				// object mesh skinned
				case NODE_OBJECT_MESH_SKINNED: {
					ObjectMeshSkinned mesh = object;
					stream.writeInt(mesh.getNumBones());
					forloop(int i = 0; mesh.getNumBones()) {
						stream.writeMat44(mesh.getBoneTransform(i));
					}
					break;
				}
				
				// object particles
				case NODE_OBJECT_PARTICLES: {
					ObjectParticles particles = object;
					stream.writeInt(particles.getSeed());
					stream.writeChar(particles.getParticlesType());
					int flags = 0;
					if(particles.getWarming()) flags |= (1 << 0);
					if(particles.getDepthSort()) flags |= (1 << 1);
					if(particles.getVariationX()) flags |= (1 << 2);
					if(particles.getVariationY()) flags |= (1 << 3);
					if(particles.getTextureAtlas()) flags |= (1 << 4);
					if(particles.getIntersection()) flags |= (1 << 5);
					if(particles.getCollision()) flags |= (1 << 6);
					if(particles.getCulling()) flags |= (1 << 7);
					stream.writeChar(flags);
					stream.writeFloat(particles.getSpawnRate());
					stream.writeFloat(particles.getSpawnScale());
					stream.writeFloat(particles.getSpawnThreshold());
					stream.writeInt(particles.getPhysicalMask());
					stream.writeFloat(particles.getPhysicalMass());
					stream.writeFloat(particles.getLengthStretch());
					stream.writeFloat(particles.getLinearDamping());
					stream.writeFloat(particles.getAngularDamping());
					stream.writeFloat(particles.getGrowthDamping());
					stream.writeFloat(particles.getRestitution());
					stream.writeFloat(particles.getRoughness());
					stream.writeChar(particles.getEmitterType());
					flags = 0;
					if(particles.isEmitterEnabled()) flags |= (1 << 0);
					if(particles.isEmitterBased()) flags |= (1 << 1);
					if(particles.isEmitterShift()) flags |= (1 << 2);
					if(particles.isEmitterContinuous()) flags |= (1 << 3);
					stream.writeChar(flags);
					stream.writeInt(particles.getEmitterSequence());
					stream.writeInt(particles.getEmitterLimit());
					stream.writeVec3(particles.getEmitterSize());
					stream.writeVec3(particles.getEmitterDirection());
					stream.writeVec3(particles.getEmitterSpread());
					stream.writeVec3(particles.getEmitterVelocity());
					stream.writeFloat(particles.getDelayMean());
					stream.writeFloat(particles.getDelaySpread());
					stream.writeFloat(particles.getPeriodMean());
					stream.writeFloat(particles.getPeriodSpread());
					stream.writeFloat(particles.getDurationMean());
					stream.writeFloat(particles.getDurationSpread());
					stream.writeFloat(particles.getLifeMean());
					stream.writeFloat(particles.getLifeSpread());
					stream.writeFloat(particles.getVelocityMean());
					stream.writeFloat(particles.getVelocitySpread());
					stream.writeFloat(particles.getAngleMean());
					stream.writeFloat(particles.getAngleSpread());
					stream.writeFloat(particles.getRotationMean());
					stream.writeFloat(particles.getRotationSpread());
					stream.writeFloat(particles.getRadiusMean());
					stream.writeFloat(particles.getRadiusSpread());
					stream.writeFloat(particles.getGrowthMean());
					stream.writeFloat(particles.getGrowthSpread());
					stream.writeVec3(particles.getGravity());
					stream.writeInt(particles.getNumForces());
					forloop(int i = 0; particles.getNumForces()) {
						stream.writeChar(particles.isForceAttached(i));
						stream.writeVariable(particles.getForceTransform(i));
						stream.writeFloat(particles.getForceRadius(i));
						stream.writeFloat(particles.getForceAttenuation(i));
						stream.writeFloat(particles.getForceAttractor(i));
						stream.writeFloat(particles.getForceRotator(i));
					}
					break;
				}
				
				// object billboard
				case NODE_OBJECT_BILLBOARD: {
					ObjectBillboard billboard = object;
					stream.writeFloat(billboard.getWidth());
					stream.writeFloat(billboard.getHeight());
					stream.writeVec4(billboard.getTexCoord());
					break;
				}
				
				// object volume sphere
				case NODE_OBJECT_VOLUME_BOX: {
					ObjectVolumeBox box = object;
					stream.writeVec3(box.getSize());
					break;
				}
				
				// object volume sphere
				case NODE_OBJECT_VOLUME_SPHERE: {
					ObjectVolumeSphere sphere = object;
					stream.writeVec3(sphere.getRadius());
					break;
				}
				
				// object volume omni
				case NODE_OBJECT_VOLUME_OMNI: {
					ObjectVolumeOmni omni = object;
					stream.writeFloat(omni.getWidth());
					stream.writeFloat(omni.getHeight());
					stream.writeFloat(omni.getRadius());
					stream.writeFloat(omni.getAttenuation());
					break;
				}
				
				// object volume proj
				case NODE_OBJECT_VOLUME_PROJ: {
					ObjectVolumeProj proj = object;
					stream.writeFloat(proj.getSize());
					stream.writeFloat(proj.getRadius());
					stream.writeFloat(proj.getFov());
					stream.writeFloat(proj.getStep());
					stream.writeFloat(proj.getVelocity());
					stream.writeFloat(proj.getRotation());
					break;
				}
				
				// object water
				case NODE_OBJECT_WATER: {
					ObjectWater water = object;
					stream.writeVec4(water.getWave(0));
					stream.writeVec4(water.getWave(1));
					stream.writeVec4(water.getWave(2));
					stream.writeVec4(water.getWave(3));
					break;
				}
				
				// object sky
				case NODE_OBJECT_SKY: {
					ObjectSky sky = object;
					stream.writeFloat(sky.getHumidity());
					stream.writeFloat(sky.getTransition());
					stream.writeFloat(sky.getExtinction());
					stream.writeInt(sky.getDensityLayer());
					break;
				}
			}
		}
		
		/*
		 */
		void restore_object(Object object,Stream stream) {
			
			// surface parameters
			int num_surfaces = object.getNumSurfaces();
			forloop(int i = 0; stream.readInt()) {
				int flags = stream.readChar();
				string material = stream.readString();
				if(i < num_surfaces) {
					object.setEnabled((flags & (1 << 0)) != 0,i);
					object.setCastShadow((flags & (1 << 1)) != 0,i);
					object.setReceiveShadow((flags & (1 << 2)) != 0,i);
					object.setCastWorldShadow((flags & (1 << 3)) != 0,i);
					object.setReceiveWorldShadow((flags & (1 << 4)) != 0,i);
					if(object.getMaterialName(i) != material) object.setMaterial(material,i);
				}
			}
			
			// type parameters
			switch(object.getType()) {
				
				// object mesh skinned
				case NODE_OBJECT_MESH_SKINNED: {
					ObjectMeshSkinned mesh = object;
					int num_bones = mesh.getNumBones();
					forloop(int i = 0; stream.readInt()) {
						mat4 transform = stream.readMat44();
						if(i < num_bones) mesh.setBoneTransform(i,transform);
					}
					break;
				}
				
				// object particles
				case NODE_OBJECT_PARTICLES: {
					ObjectParticles particles = object;
					particles.setSeed(stream.readInt());
					particles.setParticlesType(stream.readChar());
					int flags = stream.readChar();
					particles.setWarming((flags & (1 << 0)) != 0);
					particles.setDepthSort((flags & (1 << 1)) != 0);
					particles.setVariationX((flags & (1 << 2)) != 0);
					particles.setVariationY((flags & (1 << 3)) != 0);
					particles.setTextureAtlas((flags & (1 << 4)) != 0);
					particles.setIntersection((flags & (1 << 5)) != 0);
					particles.setCollision((flags & (1 << 6)) != 0);
					particles.setCulling((flags & (1 << 7)) != 0);
					particles.setSpawnRate(stream.readFloat());
					particles.setSpawnScale(stream.readFloat());
					particles.setSpawnThreshold(stream.readFloat());
					particles.setPhysicalMask(stream.readInt());
					particles.setPhysicalMass(stream.readFloat());
					particles.setLengthStretch(stream.readFloat());
					particles.setLinearDamping(stream.readFloat());
					particles.setAngularDamping(stream.readFloat());
					particles.setGrowthDamping(stream.readFloat());
					particles.setRestitution(stream.readFloat());
					particles.setRoughness(stream.readFloat());
					particles.setEmitterType(stream.readChar());
					flags = stream.readChar();
					particles.setEmitterEnabled((flags & (1 << 0)) != 0);
					particles.setEmitterBased((flags & (1 << 1)) != 0);
					particles.setEmitterShift((flags & (1 << 2)) != 0);
					particles.setEmitterContinuous((flags & (1 << 3)) != 0);
					particles.setEmitterSequence(stream.readInt());
					particles.setEmitterLimit(stream.readInt());
					particles.setEmitterSize(stream.readVec3());
					particles.setEmitterDirection(stream.readVec3());
					particles.setEmitterSpread(stream.readVec3());
					particles.setEmitterVelocity(stream.readVec3());
					float mean = stream.readFloat();
					float spread = stream.readFloat();
					particles.setDelay(mean,spread);
					mean = stream.readFloat();
					spread = stream.readFloat();
					particles.setPeriod(mean,spread);
					mean = stream.readFloat();
					spread = stream.readFloat();
					particles.setDuration(mean,spread);
					mean = stream.readFloat();
					spread = stream.readFloat();
					particles.setLife(mean,spread);
					mean = stream.readFloat();
					spread = stream.readFloat();
					particles.setVelocity(mean,spread);
					mean = stream.readFloat();
					spread = stream.readFloat();
					particles.setAngle(mean,spread);
					mean = stream.readFloat();
					spread = stream.readFloat();
					particles.setRotation(mean,spread);
					mean = stream.readFloat();
					spread = stream.readFloat();
					particles.setRadius(mean,spread);
					mean = stream.readFloat();
					spread = stream.readFloat();
					particles.setGrowth(mean,spread);
					particles.setGravity(stream.readVec3());
					while(particles.getNumForces() > 0) {
						particles.removeForce(particles.getNumForces() - 1);
					}
					forloop(int i = 0; stream.readInt()) {
						int force = particles.addForce();
						particles.setForceAttached(force,stream.readChar());
						particles.setForceTransform(force,stream.readVariable());
						particles.setForceRadius(force,stream.readFloat());
						particles.setForceAttenuation(force,stream.readFloat());
						particles.setForceAttractor(force,stream.readFloat());
						particles.setForceRotator(force,stream.readFloat());
					}
					break;
				}
				
				// object billboard
				case NODE_OBJECT_BILLBOARD: {
					ObjectBillboard billboard = object;
					float width = stream.readFloat();
					float height = stream.readFloat();
					billboard.setSize(width,height);
					billboard.setTexCoord(stream.readVec4());
					break;
				}
				
				// object volume sphere
				case NODE_OBJECT_VOLUME_BOX: {
					ObjectVolumeBox box = object;
					box.setSize(stream.readVec3());
					break;
				}
				
				// object volume sphere
				case NODE_OBJECT_VOLUME_SPHERE: {
					ObjectVolumeSphere sphere = object;
					sphere.setRadius(stream.readVec3());
					break;
				}
				
				// object volume omni
				case NODE_OBJECT_VOLUME_OMNI: {
					ObjectVolumeOmni omni = object;
					float width = stream.readFloat();
					float height = stream.readFloat();
					omni.setSize(width,height);
					omni.setRadius(stream.readFloat());
					omni.setAttenuation(stream.readFloat());
					break;
				}
				
				// object volume proj
				case NODE_OBJECT_VOLUME_PROJ: {
					ObjectVolumeProj proj = object;
					proj.setSize(stream.readFloat());
					proj.setRadius(stream.readFloat());
					proj.setFov(stream.readFloat());
					proj.setStep(stream.readFloat());
					proj.setVelocity(stream.readFloat());
					proj.setRotation(stream.readFloat());
					break;
				}
				
				// object water
				case NODE_OBJECT_WATER: {
					ObjectWater water = object;
					water.setWave(0,stream.readVec4());
					water.setWave(1,stream.readVec4());
					water.setWave(2,stream.readVec4());
					water.setWave(3,stream.readVec4());
					break;
				}
				
				// object sky
				case NODE_OBJECT_SKY: {
					ObjectSky sky = object;
					sky.setHumidity(stream.readFloat());
					sky.setTransition(stream.readFloat());
					sky.setExtinction(stream.readFloat());
					sky.setDensityLayer(stream.readInt());
					break;
				}
			}
		}
	}
	
} /* namespace Unigine::Syncker */

#endif /* __UNIGINE_SYNCKER_SERIALIZER_H__ */
