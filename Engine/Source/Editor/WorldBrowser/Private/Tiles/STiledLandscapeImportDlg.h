// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

/////////////////////////////////////////////////////
// STiledLandcapeImportDlg
// 
class STiledLandcapeImportDlg
	: public SCompoundWidget
{
public:	
	/** */		
	struct FTileImportConfiguration
	{
		int32 Resolution;
		
		int32 NumComponents;
		int32 NumSectionsPerComponent;
		int32 NumQuadsPerSection;
	};

	SLATE_BEGIN_ARGS( STiledLandcapeImportDlg )
		{}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, TSharedPtr<SWindow> InParentWindow);

	/** */
	bool ShouldImport() const;	
	const FTiledLandscapeImportSettings& GetImportSettings() const;

private:
	/** */
	TSharedRef<SWidget> CreateLandscapeMaterialPicker();
	FText GetLandscapeMaterialName() const;

	/** */
	TSharedRef<SWidget> HandleTileConfigurationComboBoxGenarateWidget(TSharedPtr<FTileImportConfiguration> InItem) const;
	FText				GetTileConfigurationText() const;
	
	/** */
	TSharedRef<ITableRow> OnGenerateWidgetForLayerDataListView(TSharedPtr<FTiledLandscapeImportSettings::LandscapeLayerSettings> InLayerData, const TSharedRef<STableViewBase>& OwnerTable);

	/** */
	TOptional<float> GetScaleX() const;
	TOptional<float> GetScaleY() const;
	TOptional<float> GetScaleZ() const;
	void OnSetScale(float InValue, ETextCommit::Type CommitType, int32 InAxis);

	/** */
	TOptional<int32> GetTileOffsetX() const;
	TOptional<int32> GetTileOffsetY() const;
	void SetTileOffsetX(int32 InValue);
	void SetTileOffsetY(int32 InValue);
	
	/** */
	void OnSetImportConfiguration(TSharedPtr<FTileImportConfiguration> InTileConfig, ESelectInfo::Type SelectInfo);

	/** */
	bool IsImportEnabled() const;
		
	/** */
	FReply OnClickedSelectHeightmapTiles();
	FReply OnClickedSelectWeighmapTiles(TSharedPtr<FTiledLandscapeImportSettings::LandscapeLayerSettings> InLayerData);
	FReply OnClickedImport();
	FReply OnClickedCancel();

	/** */
	void OnLandscapeMaterialChanged(const FAssetData& AssetData);

	/** */
	FText GetImportSummaryText() const;
	FText GetWeightmapCountText(TSharedPtr<FTiledLandscapeImportSettings::LandscapeLayerSettings> InLayerData) const; 

	/** */
	ESlateCheckBoxState::Type GetLayerBlendState(TSharedPtr<FTiledLandscapeImportSettings::LandscapeLayerSettings> InLayerData) const;
	void OnLayerBlendStateChanged(ESlateCheckBoxState::Type NewState, TSharedPtr<FTiledLandscapeImportSettings::LandscapeLayerSettings> InLayerData);

	/** */
	void SetPossibleConfigurationsForResolution(int32 TargetResolutuion);
	
	/** */
	void GenerateAllPossibleTileConfigurations();

	/** */
	FText GenerateConfigurationText(int32 NumComponents, int32 NumSectionsPerComponent,	int32 NumQuadsPerSection) const;

	/** */
	void UpdateLandscapeLayerList();

private:
	/** */
	bool bShouldImport;
	
	/** */
	TSharedPtr<SWindow> ParentWindow;
		
	/** */
	TSharedPtr<SComboBox<TSharedPtr<FTileImportConfiguration>>> TileConfigurationComboBox;
	
	/** */
	TSharedPtr<SComboButton> LandscapeMaterialComboButton;

	TSharedPtr<SListView<TSharedPtr<FTiledLandscapeImportSettings::LandscapeLayerSettings>>>	LayerDataListView;
	TArray<TSharedPtr<FTiledLandscapeImportSettings::LandscapeLayerSettings>>					LayerDataList;		
		
	/** */
	FTiledLandscapeImportSettings ImportSettings;
	
	/** */
	FIntRect TotalLandscapeRect;	
	
	TArray<FTileImportConfiguration> AllConfigurations;
	TArray<TSharedPtr<FTileImportConfiguration>> ActiveConfigurations;
};

