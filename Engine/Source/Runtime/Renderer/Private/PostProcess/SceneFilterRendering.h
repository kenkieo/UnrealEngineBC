// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	SceneFilterRendering.h: Filter rendering definitions.
=============================================================================*/

#ifndef _INC_SCENEFILTERRENDERING
#define _INC_SCENEFILTERRENDERING

#define MAX_FILTER_SAMPLES	32

#include "SceneRenderTargets.h"

/**
 * Draws a quad with the given vertex positions and UVs in denormalized pixel/texel coordinates.
 * The platform-dependent mapping from pixels to texels is done automatically.
 * Note that the positions are affected by the current viewport.
 * NOTE: DrawRectangle should be used in the vertex shader to calculate the correct position and uv for vertices.
 *
 * X, Y							Position in screen pixels of the top left corner of the quad
 * SizeX, SizeY					Size in screen pixels of the quad
 * U, V							Position in texels of the top left corner of the quad's UV's
 * SizeU, SizeV					Size in texels of the quad's UV's
 * TargetSizeX, TargetSizeY		Size in screen pixels of the target surface
 * TextureSize                  Size in texels of the source texture
 * VertexShader					The vertex shader used for rendering
 * Flags						see EDrawRectangleFlags
 */
extern void DrawRectangle(
	FRHICommandList& RHICmdList,
	float X,
	float Y,
	float SizeX,
	float SizeY,
	float U,
	float V,
	float SizeU,
	float SizeV,
	FIntPoint TargetSize,
	FIntPoint TextureSize,
	class FShader* VertexShader,
	EDrawRectangleFlags Flags = EDRF_Default
	);

extern void DrawTransformedRectangle(
	FRHICommandListImmediate& RHICmdList,
    float X,
    float Y,
    float SizeX,
    float SizeY,
    const FMatrix& PosTransform,
    float U,
    float V,
    float SizeU,
    float SizeV,
    const FMatrix& TexTransform,
    FIntPoint TargetSize,
    FIntPoint TextureSize
    );

extern TGlobalResource<FFilterVertexDeclaration> GFilterVertexDeclaration;




/*-----------------------------------------------------------------------------
FMGammaShaderParameters
-----------------------------------------------------------------------------*/

/** Encapsulates the gamma correction parameters. */
class FGammaShaderParameters
{
public:

	/** Default constructor. */
	FGammaShaderParameters() {}

	/** Initialization constructor. */
	FGammaShaderParameters(const FShaderParameterMap& ParameterMap)
	{
		RenderTargetExtent.Bind(ParameterMap,TEXT("RenderTargetExtent"));
		GammaColorScaleAndInverse.Bind(ParameterMap,TEXT("GammaColorScaleAndInverse"));
		GammaOverlayColor.Bind(ParameterMap,TEXT("GammaOverlayColor"));
	}

	/** Set the material shader parameter values. */
	void Set(FRHICommandList& RHICmdList, FShader* PixelShader, float DisplayGamma, FLinearColor const& ColorScale, FLinearColor const& ColorOverlay)
	{
		// GammaColorScaleAndInverse

		float InvDisplayGamma = 1.f / FMath::Max<float>(DisplayGamma,KINDA_SMALL_NUMBER);
		float OneMinusOverlayBlend = 1.f - ColorOverlay.A;

		FVector4 ColorScaleAndInverse;

		ColorScaleAndInverse.X = ColorScale.R * OneMinusOverlayBlend;
		ColorScaleAndInverse.Y = ColorScale.G * OneMinusOverlayBlend;
		ColorScaleAndInverse.Z = ColorScale.B * OneMinusOverlayBlend;
		ColorScaleAndInverse.W = InvDisplayGamma;

		SetShaderValue(
			RHICmdList, 
			PixelShader->GetPixelShader(),
			GammaColorScaleAndInverse,
			ColorScaleAndInverse
			);

		// GammaOverlayColor

		FVector4 OverlayColor;

		OverlayColor.X = ColorOverlay.R * ColorOverlay.A;
		OverlayColor.Y = ColorOverlay.G * ColorOverlay.A;
		OverlayColor.Z = ColorOverlay.B * ColorOverlay.A; 
		OverlayColor.W = 0.f; // Unused

		SetShaderValue(
			RHICmdList, 
			PixelShader->GetPixelShader(),
			GammaOverlayColor,
			OverlayColor
			);

		FIntPoint BufferSize = GSceneRenderTargets.GetBufferSizeXY();
		float BufferSizeX = (float)BufferSize.X;
		float BufferSizeY = (float)BufferSize.Y;
		float InvBufferSizeX = 1.0f / BufferSizeX;
		float InvBufferSizeY = 1.0f / BufferSizeY;

		const FVector4 vRenderTargetExtent(BufferSizeX, BufferSizeY,  InvBufferSizeX, InvBufferSizeY);

		SetShaderValue(
			RHICmdList, 
			PixelShader->GetPixelShader(),
			RenderTargetExtent, 
			vRenderTargetExtent);
	}

	/** Serializer. */
	friend FArchive& operator<<(FArchive& Ar,FGammaShaderParameters& P)
	{
		Ar << P.GammaColorScaleAndInverse;
		Ar << P.GammaOverlayColor;
		Ar << P.RenderTargetExtent;

		return Ar;
	}


private:
	FShaderParameter				GammaColorScaleAndInverse;
	FShaderParameter				GammaOverlayColor;
	FShaderParameter				RenderTargetExtent;
};

#endif
