// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "AITestSuitePrivatePCH.h"
#include "MockAI_BT.h"
#include "BehaviorTree/TestBTDecorator_CantExecute.h"

#define LOCTEXT_NAMESPACE "AITestSuite_BTTest"

struct FAITest_BTBasicSelector : public FAITest_SimpleBT
{
	FAITest_BTBasicSelector()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Failed);

			FBTBuilder::AddTask(CompNode, 1, EBTNodeResult::Succeeded);
			{
				FBTBuilder::WithDecorator<UTestBTDecorator_CantExecute>(CompNode);
			}

			FBTBuilder::AddTask(CompNode, 2, EBTNodeResult::Succeeded, 2);

			FBTBuilder::AddTask(CompNode, 3, EBTNodeResult::Failed);
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(2);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTBasicSelector, "Engine.AI.Behavior Trees.Composite node: selector")

struct FAITest_BTBasicSequence : public FAITest_SimpleBT
{
	FAITest_BTBasicSequence()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded);

			FBTBuilder::AddTask(CompNode, 1, EBTNodeResult::Failed);
			{
				FBTBuilder::WithDecorator<UTestBTDecorator_CantExecute>(CompNode);
				FBTBuilder::WithDecorator<UBTDecorator_ForceSuccess>(CompNode);
			}

			FBTBuilder::AddTask(CompNode, 2, EBTNodeResult::Failed, 2);

			FBTBuilder::AddTask(CompNode, 3, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(2);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTBasicSequence, "Engine.AI.Behavior Trees.Composite node: sequence")

struct FAITest_BTBasicParallelWait : public FAITest_SimpleBT
{
	FAITest_BTBasicParallelWait()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*BTAsset);
		{
			UBTCompositeNode& CompNode2 = FBTBuilder::AddParallel(CompNode, EBTParallelMode::WaitForBackground);
			{
				FBTBuilder::AddTaskLogFinish(CompNode2, 0, 10, EBTNodeResult::Succeeded, 6);

				UBTCompositeNode& CompNode3 = FBTBuilder::AddSequence(CompNode2);
				{
					FBTBuilder::AddTaskLogFinish(CompNode3, 1, 11, EBTNodeResult::Succeeded, 3);
					FBTBuilder::AddTaskLogFinish(CompNode3, 2, 12, EBTNodeResult::Succeeded, 3);
					FBTBuilder::AddTaskLogFinish(CompNode3, 3, 13, EBTNodeResult::Succeeded, 3);
				}
			}

			FBTBuilder::AddTask(CompNode, 4, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(0);
			ExpectedResult.Add(1);
			ExpectedResult.Add(11);
			ExpectedResult.Add(2);
		ExpectedResult.Add(10);
			ExpectedResult.Add(12);
			ExpectedResult.Add(3);
			ExpectedResult.Add(13);
		ExpectedResult.Add(4);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTBasicParallelWait, "Engine.AI.Behavior Trees.Composite node: parallel (wait)")

struct FAITest_BTBasicParallelAbort : public FAITest_SimpleBT
{
	FAITest_BTBasicParallelAbort()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*BTAsset);
		{
			UBTCompositeNode& CompNode2 = FBTBuilder::AddParallel(CompNode, EBTParallelMode::AbortBackground);
			{
				FBTBuilder::AddTaskLogFinish(CompNode2, 0, 10, EBTNodeResult::Succeeded, 6);

				UBTCompositeNode& CompNode3 = FBTBuilder::AddSequence(CompNode2);
				{
					FBTBuilder::AddTaskLogFinish(CompNode3, 1, 11, EBTNodeResult::Succeeded, 4);
					FBTBuilder::AddTaskLogFinish(CompNode3, 2, 12, EBTNodeResult::Succeeded, 4);
					FBTBuilder::AddTaskLogFinish(CompNode3, 3, 13, EBTNodeResult::Succeeded, 4);
				}
			}

			FBTBuilder::AddTask(CompNode, 4, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(0);
			ExpectedResult.Add(1);
			ExpectedResult.Add(11);
			ExpectedResult.Add(2);
		ExpectedResult.Add(10);
		ExpectedResult.Add(4);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTBasicParallelAbort, "Engine.AI.Behavior Trees.Composite node: parallel (abort)")

struct FAITest_BTCompositeDecorator : public FAITest_SimpleBT
{
	FAITest_BTCompositeDecorator()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded);

			FBTBuilder::AddTask(CompNode, 1, EBTNodeResult::Succeeded);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::NotSet, EBTFlowAbortMode::None, TEXT("Bool1"));
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::NotSet, EBTFlowAbortMode::None, TEXT("Bool2"));
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::Set, EBTFlowAbortMode::None, TEXT("Bool3"));
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::NotSet, EBTFlowAbortMode::None, TEXT("Bool4"));

				TArray<FBTDecoratorLogic>& CompositeOps = CompNode.Children.Last().DecoratorOps;
				CompositeOps.Add(FBTDecoratorLogic(EBTDecoratorLogic::Or, 3));
					CompositeOps.Add(FBTDecoratorLogic(EBTDecoratorLogic::Test, 0));
					CompositeOps.Add(FBTDecoratorLogic(EBTDecoratorLogic::Not, 1));
						CompositeOps.Add(FBTDecoratorLogic(EBTDecoratorLogic::And, 2));
							CompositeOps.Add(FBTDecoratorLogic(EBTDecoratorLogic::Test, 1));
							CompositeOps.Add(FBTDecoratorLogic(EBTDecoratorLogic::Test, 2));
					CompositeOps.Add(FBTDecoratorLogic(EBTDecoratorLogic::Test, 3));
			}

			FBTBuilder::AddTask(CompNode, 2, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(1);
		ExpectedResult.Add(2);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTCompositeDecorator, "Engine.AI.Behavior Trees.Composite decorator")

struct FAITest_BTAbortSelfFail : public FAITest_SimpleBT
{
	FAITest_BTAbortSelfFail()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded);

			UBTCompositeNode& CompNode2 = FBTBuilder::AddSequence(CompNode);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::NotSet, EBTFlowAbortMode::Self);

				FBTBuilder::AddTask(CompNode2, 1, EBTNodeResult::Succeeded);
				FBTBuilder::AddTaskFlagChange(CompNode2, true, EBTNodeResult::Succeeded);
				FBTBuilder::AddTask(CompNode2, 2, EBTNodeResult::Succeeded);
			}

			FBTBuilder::AddTask(CompNode, 3, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(1);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortSelfFail, "Engine.AI.Behavior Trees.Abort: self failure")

struct FAITest_BTAbortSelfSuccess : public FAITest_SimpleBT
{
	FAITest_BTAbortSelfSuccess()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded);

			UBTCompositeNode& CompNode2 = FBTBuilder::AddSequence(CompNode);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::NotSet, EBTFlowAbortMode::Self);
				FBTBuilder::WithDecorator<UBTDecorator_ForceSuccess>(CompNode);

				FBTBuilder::AddTask(CompNode2, 1, EBTNodeResult::Succeeded);
				FBTBuilder::AddTaskFlagChange(CompNode2, true, EBTNodeResult::Succeeded);
				FBTBuilder::AddTask(CompNode2, 2, EBTNodeResult::Succeeded);
			}

			FBTBuilder::AddTask(CompNode, 3, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(1);
		ExpectedResult.Add(3);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortSelfSuccess, "Engine.AI.Behavior Trees.Abort: self success")

struct FAITest_BTAbortLowerPri : public FAITest_SimpleBT
{
	FAITest_BTAbortLowerPri()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
			}

			UBTCompositeNode& CompNode2 = FBTBuilder::AddSequence(CompNode);
			{
				FBTBuilder::AddTask(CompNode2, 1, EBTNodeResult::Succeeded);
				FBTBuilder::AddTaskFlagChange(CompNode2, true, EBTNodeResult::Succeeded);
				FBTBuilder::AddTask(CompNode2, 2, EBTNodeResult::Failed);
			}

			FBTBuilder::AddTask(CompNode, 3, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(1);
		ExpectedResult.Add(0);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortLowerPri, "Engine.AI.Behavior Trees.Abort: lower priority")

struct FAITest_BTAbortMerge1 : public FAITest_SimpleBT
{
	FAITest_BTAbortMerge1()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
				FBTBuilder::WithDecorator<UTestBTDecorator_CantExecute>(CompNode);
			}

			UBTCompositeNode& CompNode2 = FBTBuilder::AddSequence(CompNode);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::NotSet, EBTFlowAbortMode::Self);

				FBTBuilder::AddTask(CompNode2, 1, EBTNodeResult::Succeeded);
				FBTBuilder::AddTaskFlagChange(CompNode2, true, EBTNodeResult::Succeeded);
			}

			FBTBuilder::AddTask(CompNode, 2, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(1);
		ExpectedResult.Add(2);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortMerge1, "Engine.AI.Behavior Trees.Abort: merge ranges 1")

struct FAITest_BTAbortMerge2 : public FAITest_SimpleBT
{
	FAITest_BTAbortMerge2()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Failed);

			FBTBuilder::AddTask(CompNode, 1, EBTNodeResult::Succeeded);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
				FBTBuilder::WithDecorator<UTestBTDecorator_CantExecute>(CompNode);
			}

			FBTBuilder::AddTask(CompNode, 2, EBTNodeResult::Succeeded);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
			}

			FBTBuilder::AddTask(CompNode, 3, EBTNodeResult::Failed);
			FBTBuilder::AddTaskFlagChange(CompNode, true, EBTNodeResult::Failed);
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(3);
		ExpectedResult.Add(2);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortMerge2, "Engine.AI.Behavior Trees.Abort: merge ranges 2")

struct FAITest_BTAbortMerge3 : public FAITest_SimpleBT
{
	FAITest_BTAbortMerge3()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			UBTCompositeNode& CompNode2 = FBTBuilder::AddSelector(CompNode);
			{
				FBTBuilder::AddTask(CompNode2, 0, EBTNodeResult::Succeeded);
				{
					FBTBuilder::WithDecorator<UTestBTDecorator_CantExecute>(CompNode2);
				}

				FBTBuilder::AddTask(CompNode2, 1, EBTNodeResult::Succeeded);
				{
					FBTBuilder::WithDecoratorBlackboard(CompNode2, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
					FBTBuilder::WithDecorator<UTestBTDecorator_CantExecute>(CompNode2);
				}
			}

			UBTCompositeNode& CompNode3 = FBTBuilder::AddSelector(CompNode);
			{
				FBTBuilder::AddTask(CompNode3, 2, EBTNodeResult::Succeeded);
				{
					FBTBuilder::WithDecoratorBlackboard(CompNode3, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
				}

				FBTBuilder::AddTaskFlagChange(CompNode3, true, EBTNodeResult::Failed);
			}

			FBTBuilder::AddTask(CompNode, 3, EBTNodeResult::Failed);
		}

		ExpectedResult.Add(2);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortMerge3, "Engine.AI.Behavior Trees.Abort: merge ranges 3")

struct FAITest_BTAbortParallelInternal : public FAITest_SimpleBT
{
	FAITest_BTAbortParallelInternal()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*BTAsset);
		{
			UBTCompositeNode& CompNode2 = FBTBuilder::AddParallel(CompNode, EBTParallelMode::WaitForBackground);
			{
				FBTBuilder::AddTask(CompNode2, 0, EBTNodeResult::Succeeded, 5);

				UBTCompositeNode& CompNode3 = FBTBuilder::AddSequence(CompNode2);
				{
					FBTBuilder::AddTask(CompNode3, 1, EBTNodeResult::Succeeded, 1);

					UBTCompositeNode& CompNode4 = FBTBuilder::AddSelector(CompNode3);
					{
						FBTBuilder::AddTask(CompNode4, 2, EBTNodeResult::Succeeded, 3);
						{
							FBTBuilder::WithDecoratorBlackboard(CompNode4, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
						}

						FBTBuilder::AddTask(CompNode4, 3, EBTNodeResult::Succeeded, 1);
					}

					FBTBuilder::AddTaskFlagChange(CompNode3, true, EBTNodeResult::Succeeded);
				}
			}

			FBTBuilder::AddTask(CompNode, 4, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(1);
		ExpectedResult.Add(3);
		ExpectedResult.Add(2);
		ExpectedResult.Add(4);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortParallelInternal, "Engine.AI.Behavior Trees.Abort: parallel internal")

struct FAITest_BTAbortParallelOut : public FAITest_SimpleBT
{
	FAITest_BTAbortParallelOut()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
			}

			UBTCompositeNode& CompNode2 = FBTBuilder::AddParallel(CompNode, EBTParallelMode::WaitForBackground);
			{
				FBTBuilder::AddTask(CompNode2, 1, EBTNodeResult::Failed, 5);

				UBTCompositeNode& CompNode3 = FBTBuilder::AddSequence(CompNode2);
				{
					FBTBuilder::AddTask(CompNode3, 2, EBTNodeResult::Succeeded, 1);
					FBTBuilder::AddTaskFlagChange(CompNode3, true, EBTNodeResult::Succeeded);
					FBTBuilder::AddTask(CompNode3, 3, EBTNodeResult::Succeeded, 1);
				}
			}

			FBTBuilder::AddTask(CompNode, 4, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(1);
		ExpectedResult.Add(2);
		ExpectedResult.Add(0);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortParallelOut, "Engine.AI.Behavior Trees.Abort: parallel out")

struct FAITest_BTAbortParallelOutAndBack : public FAITest_SimpleBT
{
	FAITest_BTAbortParallelOutAndBack()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded);
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
				FBTBuilder::WithDecorator<UTestBTDecorator_CantExecute>(CompNode);
			}

			UBTCompositeNode& CompNode2 = FBTBuilder::AddParallel(CompNode, EBTParallelMode::WaitForBackground);
			{
				FBTBuilder::AddTask(CompNode2, 1, EBTNodeResult::Failed, 5);

				UBTCompositeNode& CompNode3 = FBTBuilder::AddSequence(CompNode2);
				{
					FBTBuilder::AddTask(CompNode3, 2, EBTNodeResult::Succeeded, 2);
					FBTBuilder::AddTaskFlagChange(CompNode3, true, EBTNodeResult::Succeeded);
					FBTBuilder::AddTask(CompNode3, 3, EBTNodeResult::Succeeded, 3);
				}
			}

			FBTBuilder::AddTask(CompNode, 4, EBTNodeResult::Succeeded);
		}

		ExpectedResult.Add(1);
		ExpectedResult.Add(2);
		ExpectedResult.Add(3);
		ExpectedResult.Add(4);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortParallelOutAndBack, "Engine.AI.Behavior Trees.Abort: parallel out & back")

struct FAITest_BTAbortMultipleDelayed : public FAITest_SimpleBT
{
	FAITest_BTAbortMultipleDelayed()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Failed);

			UBTCompositeNode& CompNode2 = FBTBuilder::AddSequence(CompNode);
			{
				FBTBuilder::WithDecoratorDelayedAbort(CompNode, 2, false);

				FBTBuilder::AddTaskLogFinish(CompNode2, 1, 11, EBTNodeResult::Succeeded, 4);
				FBTBuilder::AddTaskLogFinish(CompNode2, 2, 12, EBTNodeResult::Succeeded, 4);
			}
			
			FBTBuilder::AddTask(CompNode, 4, EBTNodeResult::Failed);
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(1);
		ExpectedResult.Add(4);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortMultipleDelayed, "Engine.AI.Behavior Trees.Abort: multiple delayed requests")

struct FAITest_BTAbortToInactiveParallel : public FAITest_SimpleBT
{
	FAITest_BTAbortToInactiveParallel()
	{
		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			UBTCompositeNode& CompNode2 = FBTBuilder::AddParallel(CompNode, EBTParallelMode::WaitForBackground);
			{
				FBTBuilder::WithDecoratorDelayedAbort(CompNode, 5);

				FBTBuilder::AddTaskLogFinish(CompNode2, 1, 11, EBTNodeResult::Succeeded, 10);

				UBTCompositeNode& CompNode3 = FBTBuilder::AddSelector(CompNode2);
				{
					FBTBuilder::AddTask(CompNode3, 2, EBTNodeResult::Succeeded);
					{
						FBTBuilder::WithDecoratorBlackboard(CompNode3, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);
					}

					FBTBuilder::AddTaskLogFinish(CompNode3, 3, 13, EBTNodeResult::Succeeded, 8);
				}
			}

			UBTCompositeNode& CompNode4 = FBTBuilder::AddSequence(CompNode);
			{
				FBTBuilder::AddTask(CompNode4, 4, EBTNodeResult::Succeeded);
				FBTBuilder::AddTaskFlagChange(CompNode4, true, EBTNodeResult::Succeeded);
				FBTBuilder::AddTask(CompNode4, 5, EBTNodeResult::Succeeded);
			}
		}

		ExpectedResult.Add(1);
		ExpectedResult.Add(3);
		ExpectedResult.Add(4);
		ExpectedResult.Add(5);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTAbortToInactiveParallel, "Engine.AI.Behavior Trees.Abort: observer in inactive parallel")

struct FAITest_BTSubtreeSimple : public FAITest_SimpleBT
{
	FAITest_BTSubtreeSimple()
	{
		UBehaviorTree* ChildAsset1 = &FBTBuilder::CreateBehaviorTree(*BTAsset);
		{
			UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*ChildAsset1);
			{
				FBTBuilder::AddTask(CompNode, 10, EBTNodeResult::Succeeded);
				FBTBuilder::AddTask(CompNode, 11, EBTNodeResult::Succeeded);
			}
		}

		UBehaviorTree* ChildAsset2 = &FBTBuilder::CreateBehaviorTree(*BTAsset);
		{
			UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*ChildAsset2);
			{
				FBTBuilder::AddTask(CompNode, 20, EBTNodeResult::Failed);
				FBTBuilder::AddTask(CompNode, 21, EBTNodeResult::Succeeded);
			}
		}

		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			UBTCompositeNode& CompNode2 = FBTBuilder::AddSelector(CompNode);
			{
				FBTBuilder::AddTask(CompNode2, 0, EBTNodeResult::Failed);
				FBTBuilder::AddTaskSubtree(CompNode2, ChildAsset2);
				FBTBuilder::AddTask(CompNode2, 1, EBTNodeResult::Failed);
			}

			UBTCompositeNode& CompNode3 = FBTBuilder::AddSequence(CompNode);
			{
				FBTBuilder::AddTask(CompNode3, 2, EBTNodeResult::Succeeded);
				FBTBuilder::AddTaskSubtree(CompNode3, ChildAsset1);
				FBTBuilder::AddTask(CompNode3, 3, EBTNodeResult::Succeeded);
			}
		}

		ExpectedResult.Add(0);
		ExpectedResult.Add(20);
		ExpectedResult.Add(1);
		ExpectedResult.Add(2);
		ExpectedResult.Add(10);
		ExpectedResult.Add(11);
		ExpectedResult.Add(3);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTSubtreeSimple, "Engine.AI.Behavior Trees.Subtree: simple")

struct FAITest_BTSubtreeAbortOut : public FAITest_SimpleBT
{
	FAITest_BTSubtreeAbortOut()
	{
		UBehaviorTree* ChildAsset = &FBTBuilder::CreateBehaviorTree(*BTAsset);
		{
			UBTCompositeNode& CompNode = FBTBuilder::AddSequence(*ChildAsset);
			{
				FBTBuilder::AddTask(CompNode, 10, EBTNodeResult::Succeeded);
				FBTBuilder::AddTaskFlagChange(CompNode, true, EBTNodeResult::Succeeded);
				FBTBuilder::AddTask(CompNode, 11, EBTNodeResult::Succeeded);
			}
		}

		UBTCompositeNode& CompNode = FBTBuilder::AddSelector(*BTAsset);
		{
			FBTBuilder::AddTask(CompNode, 0, EBTNodeResult::Succeeded); 
			{
				FBTBuilder::WithDecoratorBlackboard(CompNode, EBasicKeyOperation::Set, EBTFlowAbortMode::LowerPriority);			
			}

			FBTBuilder::AddTaskSubtree(CompNode, ChildAsset);
		}

		ExpectedResult.Add(10);
		ExpectedResult.Add(0);
	}
};
IMPLEMENT_AI_TEST(FAITest_BTSubtreeAbortOut, "Engine.AI.Behavior Trees.Subtree: abort out")

#undef LOCTEXT_NAMESPACE
