// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

class FQuestionBlock;

class SQuestionBlock : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS( SQuestionBlock )
	{ }
	SLATE_END_ARGS()


	/** Widget constructor */
	void Construct( const FArguments& Args, const TSharedRef<FQuestionBlock>& Block );

private:

	ESlateCheckBoxState::Type IsAnswerChecked( TWeakPtr< FQuestionBlock > BlockPtr, int32 QuestionIndex, int32 AnswerIndex ) const;
	void AnswerCheckStateChanged( ESlateCheckBoxState::Type CheckState, TWeakPtr< FQuestionBlock > BlockPtr, int32 QuestionIndex, int32 AnswerIndex );

	void AnswerTextChanged( const FText& Text, TWeakPtr< FQuestionBlock > BlockPtr, int32 QuestionIndex, int32 AnswerIndex ) const;
	void AnswerTextCommitted( const FText& Text, ETextCommit::Type CommitType, TWeakPtr< FQuestionBlock > BlockPtr, int32 QuestionIndex, int32 AnswerIndex ) const;
};
