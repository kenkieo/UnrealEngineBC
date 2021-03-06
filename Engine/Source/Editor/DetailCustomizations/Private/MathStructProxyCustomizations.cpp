// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "DetailCustomizationsPrivatePCH.h"
#include "MathStructProxyCustomizations.h"
#include "IPropertyUtilities.h"
#include "ScopedTransaction.h"
#include "SNumericEntryBox.h"

void FMathStructProxyCustomization::CustomizeChildren( TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils )
{
	PropertyUtilities = StructCustomizationUtils.GetPropertyUtilities();
}

void FMathStructProxyCustomization::MakeHeaderRow( TSharedRef<class IPropertyHandle>& StructPropertyHandle, FDetailWidgetRow& Row )
{

}

template<typename ProxyType, typename NumericType>
TSharedRef<SWidget> FMathStructProxyCustomization::MakeNumericProxyWidget(TSharedRef<IPropertyHandle>& StructPropertyHandle, TSharedRef< TProxyProperty<ProxyType, NumericType> >& ProxyValue, const FText& Label, const FLinearColor& LabelColor, const FLinearColor& LabelBackgroundColor)
{
	TWeakPtr<IPropertyHandle> WeakHandlePtr = StructPropertyHandle;

	return 
		SNew( SNumericEntryBox<NumericType> )
		.Value( this, &FMathStructProxyCustomization::OnGetValue<ProxyType, NumericType>, WeakHandlePtr, ProxyValue )
		.Font( IDetailLayoutBuilder::GetDetailFont() )
		.UndeterminedString( NSLOCTEXT("PropertyEditor", "MultipleValues", "Multiple Values").ToString() )
		.OnValueCommitted( this, &FMathStructProxyCustomization::OnValueCommitted<ProxyType, NumericType>, WeakHandlePtr, ProxyValue )
		.OnValueChanged( this, &FMathStructProxyCustomization::OnValueChanged<ProxyType, NumericType>, WeakHandlePtr, ProxyValue )
		.OnBeginSliderMovement( this, &FMathStructProxyCustomization::OnBeginSliderMovement )
		.OnEndSliderMovement( this, &FMathStructProxyCustomization::OnEndSliderMovement<NumericType> )
		.LabelVAlign(VAlign_Fill)
		.LabelPadding(0)
		// Only allow spin on handles with one object.  Otherwise it is not clear what value to spin
		.AllowSpin( StructPropertyHandle->GetNumOuterObjects() == 1 )
		.MinValue(TOptional<NumericType>())
		.MaxValue(TOptional<NumericType>())
		.MaxSliderValue(TOptional<NumericType>())
		.MinSliderValue(TOptional<NumericType>())
		.Label()
		[
			SNumericEntryBox<float>::BuildLabel( Label, LabelColor, LabelBackgroundColor )
		];
}


template<typename ProxyType, typename NumericType>
TOptional<NumericType> FMathStructProxyCustomization::OnGetValue( TWeakPtr<IPropertyHandle> WeakHandlePtr, TSharedRef< TProxyProperty<ProxyType, NumericType> > ProxyValue ) const
{
	if(CacheValues(WeakHandlePtr))
	{
		return ProxyValue->Get();
	}
	return TOptional<NumericType>();
}

template<typename ProxyType, typename NumericType>
void FMathStructProxyCustomization::OnValueCommitted( NumericType NewValue, ETextCommit::Type CommitType, TWeakPtr<IPropertyHandle> WeakHandlePtr, TSharedRef< TProxyProperty<ProxyType, NumericType> > ProxyValue )
{
	ProxyValue->Set(NewValue);
	FlushValues(WeakHandlePtr);
}	

template<typename ProxyType, typename NumericType>
void FMathStructProxyCustomization::OnValueChanged( NumericType NewValue, TWeakPtr<IPropertyHandle> WeakHandlePtr, TSharedRef< TProxyProperty<ProxyType, NumericType> > ProxyValue )
{
	if( bIsUsingSlider )
	{
		ProxyValue->Set(NewValue);
		FlushValues(WeakHandlePtr);
	}
}

void FMathStructProxyCustomization::OnBeginSliderMovement()
{
	bIsUsingSlider = true;

	GEditor->BeginTransaction( NSLOCTEXT("FMathStructCustomization", "SetVectorProperty", "Set Vector Property") );
}

template<typename NumericType>
void FMathStructProxyCustomization::OnEndSliderMovement( NumericType NewValue )
{
	bIsUsingSlider = false;

	GEditor->EndTransaction();
}


#define LOCTEXT_NAMESPACE "MatrixStructCustomization"

TSharedRef<IPropertyTypeCustomization> FMatrixStructCustomization::MakeInstance()
{
	return MakeShareable( new FMatrixStructCustomization );
}

void FMatrixStructCustomization::MakeHeaderRow(TSharedRef<class IPropertyHandle>& StructPropertyHandle, FDetailWidgetRow& Row)
{
	Row
	.NameContent()
	[
		StructPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(0.0f)
	.MaxDesiredWidth(0.0f)
	[
		SNullWidget::NullWidget
	];
}

void FMatrixStructCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	FMathStructProxyCustomization::CustomizeChildren(StructPropertyHandle, StructBuilder, StructCustomizationUtils);

	TWeakPtr<IPropertyHandle> WeakHandlePtr = StructPropertyHandle;

	StructBuilder.AddChildContent(LOCTEXT("LocationLabel", "Location").ToString())
		.CopyAction(FUIAction(FExecuteAction::CreateSP(this, &FMatrixStructCustomization::OnCopy, FTransformField::Location, WeakHandlePtr)))
		.PasteAction(FUIAction(FExecuteAction::CreateSP(this, &FMatrixStructCustomization::OnPaste, FTransformField::Location, WeakHandlePtr)))
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget(LOCTEXT("LocationLabel", "Location").ToString())
		]
		.ValueContent()
		.MinDesiredWidth(375.0f)
		.MaxDesiredWidth(375.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 3.0f, 2.0f))
			[
				MakeNumericProxyWidget<FVector, float>(StructPropertyHandle, CachedTranslationX, LOCTEXT("TranslationX", "X"), FLinearColor::White, SNumericEntryBox<float>::RedLabelBackgroundColor)
			]
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 3.0f, 2.0f))
			[
				MakeNumericProxyWidget<FVector, float>(StructPropertyHandle, CachedTranslationY, LOCTEXT("TranslationY", "Y"), FLinearColor::White, SNumericEntryBox<float>::GreenLabelBackgroundColor)
			]
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 0.0f, 2.0f))
			[
				MakeNumericProxyWidget<FVector, float>(StructPropertyHandle, CachedTranslationZ, LOCTEXT("TranslationZ", "Z"), FLinearColor::White, SNumericEntryBox<float>::BlueLabelBackgroundColor)
			]
		];

	StructBuilder.AddChildContent(LOCTEXT("RotationLabel", "Rotation").ToString())
		.CopyAction(FUIAction(FExecuteAction::CreateSP(this, &FMatrixStructCustomization::OnCopy, FTransformField::Rotation, WeakHandlePtr)))
		.PasteAction(FUIAction(FExecuteAction::CreateSP(this, &FMatrixStructCustomization::OnPaste, FTransformField::Rotation, WeakHandlePtr)))
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget(LOCTEXT("RotationLabel", "Rotation").ToString())
		]
		.ValueContent()
		.MinDesiredWidth(375.0f)
		.MaxDesiredWidth(375.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 3.0f, 2.0f))
			[
				MakeNumericProxyWidget<FRotator, float>(StructPropertyHandle, CachedRotationRoll, LOCTEXT("RotationRoll", "X"), FLinearColor::White, SNumericEntryBox<float>::RedLabelBackgroundColor)
			]
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 3.0f, 2.0f))
			[
				MakeNumericProxyWidget<FRotator, float>(StructPropertyHandle, CachedRotationPitch, LOCTEXT("RotationPitch", "Y"), FLinearColor::White, SNumericEntryBox<float>::GreenLabelBackgroundColor)
			]
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 0.0f, 2.0f))
			[
				MakeNumericProxyWidget<FRotator, float>(StructPropertyHandle, CachedRotationYaw, LOCTEXT("RotationYaw", "Z"), FLinearColor::White, SNumericEntryBox<float>::BlueLabelBackgroundColor)
			]
		];

	StructBuilder.AddChildContent(LOCTEXT("ScaleLabel", "Scale").ToString())
		.CopyAction(FUIAction(FExecuteAction::CreateSP(this, &FMatrixStructCustomization::OnCopy, FTransformField::Scale, WeakHandlePtr)))
		.PasteAction(FUIAction(FExecuteAction::CreateSP(this, &FMatrixStructCustomization::OnPaste, FTransformField::Scale, WeakHandlePtr)))
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget(LOCTEXT("ScaleLabel", "Scale").ToString())
		]
		.ValueContent()
		.MinDesiredWidth(375.0f)
		.MaxDesiredWidth(375.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 3.0f, 2.0f))
			[
				MakeNumericProxyWidget<FVector, float>(StructPropertyHandle, CachedScaleX, LOCTEXT("ScaleX", "X"), FLinearColor::White, SNumericEntryBox<float>::RedLabelBackgroundColor)
			]
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 3.0f, 2.0f))
			[
				MakeNumericProxyWidget<FVector, float>(StructPropertyHandle, CachedScaleY, LOCTEXT("ScaleY", "Y"), FLinearColor::White, SNumericEntryBox<float>::GreenLabelBackgroundColor)
			]
			+ SHorizontalBox::Slot()
			.Padding(FMargin(0.0f, 2.0f, 0.0f, 2.0f))
			[
				MakeNumericProxyWidget<FVector, float>(StructPropertyHandle, CachedScaleZ, LOCTEXT("ScaleZ", "Z"), FLinearColor::White, SNumericEntryBox<float>::BlueLabelBackgroundColor)
			]
		];
}

void FMatrixStructCustomization::OnCopy(FTransformField::Type Type, TWeakPtr<IPropertyHandle> PropertyHandlePtr)
{
	auto PropertyHandle = PropertyHandlePtr.Pin();

	if (!PropertyHandle.IsValid())
	{
		return;
	}

	FString CopyStr;
	CacheValues(PropertyHandle);

	switch (Type)
	{
		case FTransformField::Location:
		{
			FVector Location = CachedTranslation->Get();
			CopyStr = FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), Location.X, Location.Y, Location.Z);
			break;
		}

		case FTransformField::Rotation:
		{
			FRotator Rotation = CachedRotation->Get();
			CopyStr = FString::Printf(TEXT("(Pitch=%f,Yaw=%f,Roll=%f)"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
			break;
		}

		case FTransformField::Scale:
		{
			FVector Scale = CachedScale->Get();
			CopyStr = FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), Scale.X, Scale.Y, Scale.Z);
			break;
		}
	}

	if (!CopyStr.IsEmpty())
	{
		FPlatformMisc::ClipboardCopy(*CopyStr);
	}
}

void FMatrixStructCustomization::OnPaste(FTransformField::Type Type, TWeakPtr<IPropertyHandle> PropertyHandlePtr)
{
	auto PropertyHandle = PropertyHandlePtr.Pin();

	if (!PropertyHandle.IsValid())
	{
		return;
	}

	FString PastedText;
	FPlatformMisc::ClipboardPaste(PastedText);

	switch (Type)
	{
		case FTransformField::Location:
		{
			FVector Location;
			if (Location.InitFromString(PastedText))
			{
				FScopedTransaction Transaction(LOCTEXT("PasteLocation", "Paste Location"));
				CachedTranslationX->Set(Location.X);
				CachedTranslationY->Set(Location.Y);
				CachedTranslationZ->Set(Location.Z);
				FlushValues(PropertyHandle);
			}
			break;
		}

		case FTransformField::Rotation:
		{
			FRotator Rotation;
			PastedText.ReplaceInline(TEXT("Pitch="), TEXT("P="));
			PastedText.ReplaceInline(TEXT("Yaw="), TEXT("Y="));
			PastedText.ReplaceInline(TEXT("Roll="), TEXT("R="));
			if (Rotation.InitFromString(PastedText))
			{
				FScopedTransaction Transaction(LOCTEXT("PasteRotation", "Paste Rotation"));
				CachedRotationPitch->Set(Rotation.Pitch);
				CachedRotationYaw->Set(Rotation.Yaw);
				CachedRotationRoll->Set(Rotation.Roll);
				FlushValues(PropertyHandle);
			}
			break;
		}

		case FTransformField::Scale:
		{
			FVector Scale;
			if (Scale.InitFromString(PastedText))
			{
				FScopedTransaction Transaction(LOCTEXT("PasteScale", "Paste Scale"));
				CachedScaleX->Set(Scale.X);
				CachedScaleY->Set(Scale.Y);
				CachedScaleZ->Set(Scale.Z);
				FlushValues(PropertyHandle);
			}
			break;
		}
	}
}

bool FMatrixStructCustomization::CacheValues( TWeakPtr<IPropertyHandle> PropertyHandlePtr ) const
{
	auto PropertyHandle = PropertyHandlePtr.Pin();

	if (!PropertyHandle.IsValid())
	{
		return false;
	}

	TArray<void*> RawData;
	PropertyHandle->AccessRawData(RawData);

	if (RawData.Num() == 1)
	{
		FMatrix* MatrixValue = reinterpret_cast<FMatrix*>(RawData[0]);
		if (MatrixValue != NULL)
		{
			CachedTranslation->Set(MatrixValue->GetOrigin());
			CachedRotation->Set(MatrixValue->Rotator());
			CachedScale->Set(MatrixValue->GetScaleVector());
			return true;
		}
	}

	return false;
}

bool FMatrixStructCustomization::FlushValues( TWeakPtr<IPropertyHandle> PropertyHandlePtr ) const
{
	auto PropertyHandle = PropertyHandlePtr.Pin();
	if (!PropertyHandle.IsValid())
	{
		return false;
	}

	TArray<void*> RawData;
	PropertyHandle->AccessRawData(RawData);

	PropertyHandle->NotifyPreChange();
	for (int32 ValueIndex = 0; ValueIndex < RawData.Num(); ValueIndex++)
	{
		FMatrix* MatrixValue = reinterpret_cast<FMatrix*>(RawData[ValueIndex]);
		if (MatrixValue != NULL)
		{
			const FRotator CurrentRotation = MatrixValue->Rotator();
			const FVector CurrentTranslation = MatrixValue->GetOrigin();
			const FVector CurrentScale = MatrixValue->GetScaleVector();

			FRotator Rotation(
				CachedRotationPitch->IsSet() ? CachedRotationPitch->Get() : CurrentRotation.Pitch,
				CachedRotationYaw->IsSet() ? CachedRotationYaw->Get() : CurrentRotation.Yaw,
				CachedRotationRoll->IsSet() ? CachedRotationRoll->Get() : CurrentRotation.Roll
				);
			FVector Translation(
				CachedTranslationX->IsSet() ? CachedTranslationX->Get() : CurrentTranslation.X,
				CachedTranslationY->IsSet() ? CachedTranslationY->Get() : CurrentTranslation.Y,
				CachedTranslationZ->IsSet() ? CachedTranslationZ->Get() : CurrentTranslation.Z
				);
			FVector Scale(
				CachedScaleX->IsSet() ? CachedScaleX->Get() : CurrentScale.X,
				CachedScaleY->IsSet() ? CachedScaleY->Get() : CurrentScale.Y,
				CachedScaleZ->IsSet() ? CachedScaleZ->Get() : CurrentScale.Z
				);
			*MatrixValue = FScaleRotationTranslationMatrix(Scale, Rotation, Translation);
		}
	}
	PropertyHandle->NotifyPostChange();

	return true;
}

#undef LOCTEXT_NAMESPACE

TSharedRef<IPropertyTypeCustomization> FTransformStructCustomization::MakeInstance() 
{
	return MakeShareable( new FTransformStructCustomization );
}

bool FTransformStructCustomization::CacheValues( TWeakPtr<IPropertyHandle> PropertyHandlePtr ) const
{
	auto PropertyHandle = PropertyHandlePtr.Pin();

	if (!PropertyHandle.IsValid())
	{
		return false;
	}

	TArray<void*> RawData;
	PropertyHandle->AccessRawData(RawData);

	if (RawData.Num() == 1)
	{
		FTransform* TransformValue = reinterpret_cast<FTransform*>(RawData[0]);
		if (TransformValue != NULL)
		{
			CachedTranslation->Set(TransformValue->GetTranslation());
			CachedRotation->Set(TransformValue->GetRotation().Rotator());
			CachedScale->Set(TransformValue->GetScale3D());
			return true;
		}
	}

	return false;
}

bool FTransformStructCustomization::FlushValues( TWeakPtr<IPropertyHandle> PropertyHandlePtr ) const
{
	auto PropertyHandle = PropertyHandlePtr.Pin();

	if (!PropertyHandle.IsValid())
	{
		return false;
	}

	TArray<void*> RawData;
	PropertyHandle->AccessRawData(RawData);

	PropertyHandle->NotifyPreChange();
	for (int32 ValueIndex = 0; ValueIndex < RawData.Num(); ValueIndex++)
	{
		FTransform* TransformValue = reinterpret_cast<FTransform*>(RawData[0]);
		if (TransformValue != NULL)
		{
			const FRotator CurrentRotation = TransformValue->GetRotation().Rotator();
			const FVector CurrentTranslation = TransformValue->GetTranslation();
			const FVector CurrentScale = TransformValue->GetScale3D();

			FRotator Rotation(
				CachedRotationPitch->IsSet() ? CachedRotationPitch->Get() : CurrentRotation.Pitch,
				CachedRotationYaw->IsSet() ? CachedRotationYaw->Get() : CurrentRotation.Yaw,
				CachedRotationRoll->IsSet() ? CachedRotationRoll->Get() : CurrentRotation.Roll
				);
			FVector Translation(
				CachedTranslationX->IsSet() ? CachedTranslationX->Get() : CurrentTranslation.X,
				CachedTranslationY->IsSet() ? CachedTranslationY->Get() : CurrentTranslation.Y,
				CachedTranslationZ->IsSet() ? CachedTranslationZ->Get() : CurrentTranslation.Z
				);
			FVector Scale(
				CachedScaleX->IsSet() ? CachedScaleX->Get() : CurrentScale.X,
				CachedScaleY->IsSet() ? CachedScaleY->Get() : CurrentScale.Y,
				CachedScaleZ->IsSet() ? CachedScaleZ->Get() : CurrentScale.Z
				);
			TransformValue->SetComponents(Rotation.Quaternion(), Translation, Scale);
		}
	}
	PropertyHandle->NotifyPostChange();

	if(PropertyUtilities.IsValid() && !bIsUsingSlider)
	{
		FPropertyChangedEvent ChangeEvent(PropertyHandle->GetProperty(), EPropertyChangeType::ValueSet);
		PropertyUtilities->NotifyFinishedChangingProperties(ChangeEvent);
	}

	return true;
}