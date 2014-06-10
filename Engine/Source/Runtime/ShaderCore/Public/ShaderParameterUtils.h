// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	ShaderParameters.h: Shader parameter inline definitions.
=============================================================================*/

#pragma once

#include "ShaderCore.h"
#include "ShaderParameters.h"
#include "RHICommandList.h"

/**
 * Sets the value of a  shader parameter.  Template'd on shader type
 * A template parameter specified the type of the parameter value.
 * NOTE: Shader should be the param ref type, NOT the param type, since Shader is passed by value. 
 * Otherwise AddRef/ReleaseRef will be called many times.
 */
template<typename ShaderRHIParamRef, class ParameterType>
void SetShaderValue(
	FRHICommandList& RHICmdList, 
	ShaderRHIParamRef Shader,
	const FShaderParameter& Parameter,
	const ParameterType& Value,
	uint32 ElementIndex = 0
	)
{
	checkAtCompileTime(!TIsPointerType<ParameterType>::Value, ErrorPassByValue);

	const uint32 AlignedTypeSize = Align(sizeof(ParameterType),ShaderArrayElementAlignBytes);
	const int32 NumBytesToSet = FMath::Min<int32>(sizeof(ParameterType),Parameter.GetNumBytes() - ElementIndex * AlignedTypeSize);

	// This will trigger if the parameter was not serialized
	checkSlow(Parameter.IsInitialized());

	if(NumBytesToSet > 0)
	{
		RHICmdList.SetShaderParameter(
			Shader,
			Parameter.GetBufferIndex(),
			Parameter.GetBaseIndex() + ElementIndex * AlignedTypeSize,
			(uint32)NumBytesToSet,
			&Value,
			true
			);
	}
}

/** Specialization of the above for C++ bool type. */
template<typename ShaderRHIParamRef>
void SetShaderValue(
	FRHICommandList& RHICmdList, 
	ShaderRHIParamRef Shader,
	const FShaderParameter& Parameter,
	const bool& Value,
	uint32 ElementIndex = 0
	)
{
	const uint32 BoolValue = Value;
	SetShaderValue(RHICmdList, Shader, Parameter, BoolValue, ElementIndex);
}

/**
 * Sets the value of a shader parameter array.  Template'd on shader type
 * A template parameter specified the type of the parameter value.
 * NOTE: Shader should be the param ref type, NOT the param type, since Shader is passed by value. 
 * Otherwise AddRef/ReleaseRef will be called many times.
 */
template<typename ShaderRHIParamRef,class ParameterType>
void SetShaderValueArray(
	FRHICommandList& RHICmdList,
	ShaderRHIParamRef Shader,
	const FShaderParameter& Parameter,
	const ParameterType* Values,
	uint32 NumElements,
	uint32 BaseElementIndex = 0
	)
{
	const uint32 AlignedTypeSize = Align(sizeof(ParameterType),ShaderArrayElementAlignBytes);
	const int32 NumBytesToSet = FMath::Min<int32>(NumElements * AlignedTypeSize,Parameter.GetNumBytes() - BaseElementIndex * AlignedTypeSize);

	// This will trigger if the parameter was not serialized
	checkSlow(Parameter.IsInitialized());

	if(NumBytesToSet > 0)
	{
		RHICmdList.SetShaderParameter(
			Shader,
			Parameter.GetBufferIndex(),
			Parameter.GetBaseIndex() + BaseElementIndex * AlignedTypeSize,
			(uint32)NumBytesToSet,
			Values,
			true
			);
	}
}

/** Specialization of the above for C++ bool type. */
template<typename ShaderRHIParamRef>
void SetShaderValueArray(
	FRHICommandList& RHICmdList, 
	ShaderRHIParamRef Shader,
	const FShaderParameter& Parameter,
	const bool* Values,
	uint32 NumElements,
	uint32 BaseElementIndex = 0
	)
{
	UE_LOG(LogShaders, Fatal, TEXT("SetShaderValueArray does not support bool arrays."));
}

/**
 * Sets the value of a pixel shader bool parameter.
 */
inline void SetPixelShaderBool(
	FRHICommandList& RHICmdList, 
	FPixelShaderRHIParamRef PixelShader,
	const FShaderParameter& Parameter,
	bool Value
	)
{
	// This will trigger if the parameter was not serialized
	checkSlow(Parameter.IsInitialized());

	if (Parameter.GetNumBytes() > 0)
	{
		// Convert to uint32 before passing to RHI
		uint32 BoolValue = Value;
		RHICmdList.SetShaderParameter(
			PixelShader,
			Parameter.GetBufferIndex(),
			Parameter.GetBaseIndex(),
			sizeof(BoolValue),
			&BoolValue,
			true
			);
	}
}

/**
 * Sets the value of a shader texture parameter.  Template'd on shader type
 */
template<typename ShaderTypeRHIParamRef>
FORCEINLINE void SetTextureParameter(
	FRHICommandList& RHICmdList, 
	ShaderTypeRHIParamRef Shader,
	const FShaderResourceParameter& TextureParameter,
	const FShaderResourceParameter& SamplerParameter,
	const FTexture* Texture,
	uint32 ElementIndex = 0
	)
{
	// This will trigger if the parameter was not serialized
	checkSlow(TextureParameter.IsInitialized());
	checkSlow(SamplerParameter.IsInitialized());
	if(TextureParameter.IsBound())
	{
		Texture->LastRenderTime = FApp::GetCurrentTime();

		if (ElementIndex < TextureParameter.GetNumResources())
		{
			RHICmdList.SetShaderTexture( Shader, TextureParameter.GetBaseIndex() + ElementIndex, Texture->TextureRHI);
		}
	}
	
	// @todo ue4 samplerstate Should we maybe pass in two separate values? SamplerElement and TextureElement? Or never allow an array of samplers? Unsure best
	// if there is a matching sampler for this texture array index (ElementIndex), then set it. This will help with this case:
	//			Texture2D LightMapTextures[NUM_LIGHTMAP_COEFFICIENTS];
	//			SamplerState LightMapTexturesSampler;
	// In this case, we only set LightMapTexturesSampler when ElementIndex is 0, we don't set the sampler state for all 4 textures
	// This assumes that the all textures want to use the same sampler state
	if(SamplerParameter.IsBound())
	{
		if (ElementIndex < SamplerParameter.GetNumResources())
		{
			RHICmdList.SetShaderSampler( Shader, SamplerParameter.GetBaseIndex() + ElementIndex, Texture->SamplerStateRHI);
		}
	}
}

/**
 * Sets the value of a shader texture parameter. Template'd on shader type.
 */
template<typename ShaderTypeRHIParamRef>
FORCEINLINE void SetTextureParameter(
	FRHICommandList& RHICmdList, 
	ShaderTypeRHIParamRef Shader,
	const FShaderResourceParameter& TextureParameter,
	const FShaderResourceParameter& SamplerParameter,
	FSamplerStateRHIParamRef SamplerStateRHI,
	FTextureRHIParamRef TextureRHI,
	uint32 ElementIndex = 0
	)
{
	// This will trigger if the parameter was not serialized
	checkSlow(TextureParameter.IsInitialized());
	checkSlow(SamplerParameter.IsInitialized());
	if(TextureParameter.IsBound())
	{
		if (ElementIndex < TextureParameter.GetNumResources())
		{
			RHICmdList.SetShaderTexture( Shader, TextureParameter.GetBaseIndex() + ElementIndex, TextureRHI);
		}
	}
	// @todo ue4 samplerstate Should we maybe pass in two separate values? SamplerElement and TextureElement? Or never allow an array of samplers? Unsure best
	// if there is a matching sampler for this texture array index (ElementIndex), then set it. This will help with this case:
	//			Texture2D LightMapTextures[NUM_LIGHTMAP_COEFFICIENTS];
	//			SamplerState LightMapTexturesSampler;
	// In this case, we only set LightMapTexturesSampler when ElementIndex is 0, we don't set the sampler state for all 4 textures
	// This assumes that the all textures want to use the same sampler state
	if(SamplerParameter.IsBound())
	{
		if (ElementIndex < SamplerParameter.GetNumResources())
		{
			RHICmdList.SetShaderSampler( Shader, SamplerParameter.GetBaseIndex() + ElementIndex, SamplerStateRHI);
		}
	}
}

/**
 * Sets the value of a shader surface parameter (e.g. to access MSAA samples).
 * Template'd on shader type (e.g. pixel shader or compute shader).
 */
template<typename ShaderTypeRHIParamRef>
FORCEINLINE void SetTextureParameter(
	FRHICommandList& RHICmdList, 
	ShaderTypeRHIParamRef Shader,
	const FShaderResourceParameter& Parameter,
	FTextureRHIParamRef NewTextureRHI
	)
{
	if(Parameter.IsBound())
	{
		RHICmdList.SetShaderTexture(
			Shader,
			Parameter.GetBaseIndex(),
			NewTextureRHI
			);
	}
}

/**
 * Sets the value of a shader sampler parameter. Template'd on shader type.
 */
template<typename ShaderTypeRHIParamRef>
FORCEINLINE void SetSamplerParameter(
	FRHICommandList& RHICmdList, 
	ShaderTypeRHIParamRef Shader,
	const FShaderResourceParameter& Parameter,
	FSamplerStateRHIParamRef SamplerStateRHI
	)
{
	if(Parameter.IsBound())
	{
		RHICmdList.SetShaderSampler(
			Shader,
			Parameter.GetBaseIndex(),
			SamplerStateRHI
			);
	}
}

/**
 * Sets the value of a shader resource view parameter
 * Template'd on shader type (e.g. pixel shader or compute shader).
 */
template<typename ShaderTypeRHIParamRef>
FORCEINLINE void SetSRVParameter(
	FRHICommandList& RHICmdList, 
	ShaderTypeRHIParamRef Shader,
	const FShaderResourceParameter& Parameter,
	FShaderResourceViewRHIParamRef NewShaderResourceViewRHI
	)
{
	if(Parameter.IsBound())
	{
		RHICmdList.SetShaderResourceViewParameter(
			Shader,
			Parameter.GetBaseIndex(),
			NewShaderResourceViewRHI
			);
	}
}

/**
 * Sets the value of a unordered access view parameter
 */
FORCEINLINE void SetUAVParameter(
	FRHICommandList& RHICmdList, 
	FComputeShaderRHIParamRef ComputeShader,
	const FShaderResourceParameter& Parameter,
	FUnorderedAccessViewRHIParamRef NewUnorderedAccessViewRHI
	)
{
	if(Parameter.IsBound())
	{
		RHICmdList.SetUAVParameter(
			ComputeShader,
			Parameter.GetBaseIndex(),
			NewUnorderedAccessViewRHI
			);
	}
}

template<typename TShaderRHIRef>
inline void SetUAVParameterIfCS(FRHICommandList& RHICmdList, TShaderRHIRef,const FShaderResourceParameter& UAVParameter,FUnorderedAccessViewRHIParamRef UAV)
{
}

template<> inline void SetUAVParameterIfCS<FComputeShaderRHIParamRef>(FRHICommandList& RHICmdList, FComputeShaderRHIParamRef ComputeShader,const FShaderResourceParameter& UAVParameter,FUnorderedAccessViewRHIParamRef UAV)
{
	SetUAVParameter(RHICmdList, ComputeShader,UAVParameter,UAV);
}

template<> inline void SetUAVParameterIfCS<FComputeShaderRHIRef>(FRHICommandList& RHICmdList, FComputeShaderRHIRef ComputeShader,const FShaderResourceParameter& UAVParameter,FUnorderedAccessViewRHIParamRef UAV)
{
	SetUAVParameter(RHICmdList, ComputeShader,UAVParameter,UAV);
}

template<typename TShaderRHIRef>
inline void FRWShaderParameter::SetBuffer(FRHICommandList& RHICmdList, TShaderRHIRef Shader,const FRWBuffer& RWBuffer) const
{
	SetSRVParameter(RHICmdList, Shader,SRVParameter,RWBuffer.SRV);
	SetUAVParameterIfCS(RHICmdList, Shader,UAVParameter,RWBuffer.UAV);
}

template<typename TShaderRHIRef>
inline void FRWShaderParameter::SetTexture(FRHICommandList& RHICmdList, TShaderRHIRef Shader,const FTextureRHIParamRef Texture,FUnorderedAccessViewRHIParamRef UAV) const
{
	SetTextureParameter(RHICmdList, Shader,SRVParameter,Texture);
	SetUAVParameterIfCS(RHICmdList, Shader,UAVParameter,UAV);
}

inline void FRWShaderParameter::UnsetUAV(FRHICommandList& RHICmdList, FComputeShaderRHIParamRef ComputeShader) const
{
	SetUAVParameter(RHICmdList, ComputeShader,UAVParameter,FUnorderedAccessViewRHIRef());
}


/** Sets the value of a shader uniform buffer parameter to a uniform buffer containing the struct. */
template<typename TShaderRHIRef>
inline void SetUniformBufferParameter(
	FRHICommandList& RHICmdList,
	TShaderRHIRef Shader,
	const FShaderUniformBufferParameter& Parameter,
	FUniformBufferRHIParamRef UniformBufferRHI
	)
{
	// This will trigger if the parameter was not serialized
	checkSlow(Parameter.IsInitialized());
	if(Parameter.IsBound())
	{
		RHICmdList.SetShaderUniformBuffer( Shader, Parameter.GetBaseIndex(), UniformBufferRHI);
	}
}

/** Sets the value of a shader uniform buffer parameter to a uniform buffer containing the struct. */
template<typename TShaderRHIRef,typename TBufferStruct>
inline void SetUniformBufferParameter(
	FRHICommandList& RHICmdList,
	TShaderRHIRef Shader,
	const TShaderUniformBufferParameter<TBufferStruct>& Parameter,
	const TUniformBufferRef<TBufferStruct>& UniformBufferRef
	)
{
	// This will trigger if the parameter was not serialized
	checkSlow(Parameter.IsInitialized());
	if(Parameter.IsBound())
	{
		RHICmdList.SetShaderUniformBuffer( Shader, Parameter.GetBaseIndex(), UniformBufferRef);
	}
}

/** Sets the value of a shader uniform buffer parameter to a uniform buffer containing the struct. */
template<typename TShaderRHIRef,typename TBufferStruct>
inline void SetUniformBufferParameter(
	FRHICommandList& RHICmdList,
	TShaderRHIRef Shader,
	const TShaderUniformBufferParameter<TBufferStruct>& Parameter,
	const TUniformBuffer<TBufferStruct>& UniformBuffer
	)
{
	// This will trigger if the parameter was not serialized
	checkSlow(Parameter.IsInitialized());
	if (Parameter.IsBound())
	{
		RHICmdList.SetShaderUniformBuffer( Shader, Parameter.GetBaseIndex(), UniformBuffer.GetUniformBufferRHI());
	}
}

/** Sets the value of a shader uniform buffer parameter to a value of the struct. */
template<typename TShaderRHIRef,typename TBufferStruct>
inline void SetUniformBufferParameterImmediate(
	FRHICommandList& RHICmdList,
	TShaderRHIRef Shader,
	const TShaderUniformBufferParameter<TBufferStruct>& Parameter,
	const TBufferStruct& UniformBufferValue
	)
{
	// This will trigger if the parameter was not serialized
	checkSlow(Parameter.IsInitialized());
	if(Parameter.IsBound())
	{
		RHICmdList.SetShaderUniformBuffer(
			Shader,
			Parameter.GetBaseIndex(),
			RHICreateUniformBuffer(&UniformBufferValue,TBufferStruct::StaticStruct.GetLayout(),UniformBuffer_SingleDraw)
			);
	}
}
