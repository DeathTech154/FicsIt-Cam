﻿#include "Editor/UI/FICSequencer.h"

#include "Editor/FICEditorContext.h"
#include "Editor/UI/FICDragDrop.h"
#include "Editor/UI/FICEditorStyle.h"
#include "Editor/UI/FICSequencerTreeView.h"

const FName FFICSequencerStyle::TypeName = "Sequencer";

const FFICSequencerStyle& FFICSequencerStyle::GetDefault() {
	static FFICSequencerStyle* Default = nullptr;
	if (!Default) {
		Default = new FFICSequencerStyle();
		*Default = FFICEditorStyles::Get().GetWidgetStyle<FFICSequencerStyle>("Sequencer");
		Default->KeyframeIcon = FFICKeyframeIconStyle::GetDefault();
	}
	return *Default;
}

void SFICSequencer::Construct(const FArguments& InArgs, UFICEditorContext* InContext, SFICSequencerTreeView* InTreeView) {
	Context = InContext;
	TreeView = InTreeView;

	Style = InArgs._Style;
	ActiveFrame = InArgs._Frame;
	FrameRange = InArgs._FrameRange;
	FrameHighlightRange = InArgs._FrameHighlightRange;
		
	Context->OnCurrentFrameChanged.AddRaw(this, &SFICSequencer::FrameRangeChanged);

	UpdateRows();
}

SFICSequencer::SFICSequencer() : Children(this) {
	Clipping = EWidgetClipping::ClipToBoundsAlways;
}

SFICSequencer::~SFICSequencer() {}

FVector2D SFICSequencer::ComputeDesiredSize(float) const {
	return FVector2D(10, 10);
}

int32 SFICSequencer::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const {
	return SPanel::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

FReply SFICSequencer::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	return SPanel::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SFICSequencer::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	return SPanel::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SFICSequencer::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) {
	return SPanel::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
}

FReply SFICSequencer::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	return SPanel::OnDragDetected(MyGeometry, MouseEvent);
}

FReply SFICSequencer::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	return SPanel::OnMouseMove(MyGeometry, MouseEvent);
}

FReply SFICSequencer::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	return SPanel::OnMouseWheel(MyGeometry, MouseEvent);
}

FReply SFICSequencer::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) {
	return SPanel::OnKeyDown(MyGeometry, InKeyEvent);
}

FReply SFICSequencer::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) {
	return SPanel::OnKeyUp(MyGeometry, InKeyEvent);
}

bool SFICSequencer::IsInteractable() const {
	return SPanel::IsInteractable();
}

FChildren* SFICSequencer::GetChildren() {
	return &Children;
}

void SFICSequencer::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const {
	for (int32 i = 0; i < Children.Num(); ++i) {
		const TSharedPtr<SFICSequencerRow>& Child = Children[i];
		const TSharedPtr<ITableRow>& Row = LinearRows[i];
		FGeometry TableRowGeometry = Row->AsWidget()->GetPaintSpaceGeometry();
		FVector2D Size = FVector2D(AllottedGeometry.GetLocalSize().X, TableRowGeometry.GetLocalSize().Y);
		FVector2D ParentOffset = FVector2D(0, AllottedGeometry.AbsoluteToLocal(TableRowGeometry.GetAbsolutePosition()).Y);
		ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(Child.ToSharedRef(), ParentOffset, Size, 1));
	}
}

void SFICSequencer::UpdateRows() {
	TArray<TSharedPtr<ITableRow>> Rows = TreeView->GetVisibleTableRows();

	TSet<int32> ToRemove;
	int32 i = 0;
	for (; i < Rows.Num(); ++i) {
		const TSharedPtr<ITableRow>& Row = Rows[i];
		TSharedPtr<FFICSequencerRowMeta> Meta = *TreeView->ItemFromWidget(Row.Get());
		
		// Skip if Item already exists
		if (LinearRows.Num() > i && LinearRows[i] == Row) continue;

		if (LinearRows.Contains(Row)) {
			// If Current Item already exists, but was not at same index -> Item to delete in LinearRows
			while (LinearRows[i] != Row) {
				TSharedPtr<SFICSequencerRow> Widget = Children[i];
				Children.RemoveAt(i);
				LinearRows.RemoveAt(i);
				MetaToWidget.Remove(WidgetToMeta[Widget]);
				WidgetToMeta.Remove(Widget);
			}
		} else {
			// If Current Item does not exist -> Create and Add Item
			TSharedPtr<SFICSequencerRow> Widget = Meta->Provider->CreateRow(this);
			Children.Insert(Widget.ToSharedRef(), i);
			LinearRows.Insert(Row, i);
			MetaToWidget.Add(Meta, Widget);
			WidgetToMeta.Add(Widget, Meta);

			Widget->UpdateActiveFrame(ActiveFrame.Get());
			Widget->UpdateFrameRange(FrameRange.Get());
		}
	}
	for (; i < LinearRows.Num();) {
		TSharedPtr<SFICSequencerRow> Widget = Children[i];
		Children.RemoveAt(i);
		LinearRows.RemoveAt(i);
		MetaToWidget.Remove(WidgetToMeta[Widget]);
		WidgetToMeta.Remove(Widget);
	}

	Invalidate(EInvalidateWidgetReason::Layout);
}

int32 SFICSequencer::GetRowIndexByWidget(TSharedRef<SFICSequencerRow> InWidget) {
	return TreeView->GetRowIndex(WidgetToMeta[InWidget]);
}

void SFICSequencer::FrameRangeChanged() {
	for (int i = 0; i < Children.Num(); ++i) {
		Children[i]->UpdateFrameRange(FrameRange.Get());
	}
}

void SFICSequencer::ActiveFrameChanged() {
	for (int i = 0; i < Children.Num(); ++i) {
		Children[i]->UpdateActiveFrame(ActiveFrame.Get());
	}
}


