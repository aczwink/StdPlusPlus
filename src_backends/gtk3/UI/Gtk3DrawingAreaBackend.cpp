/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include "Gtk3DrawingAreaBackend.hpp"
//Local
#include <Std++/UI/Events/PaintEvent.hpp>
#include <Std++/UI/Widget.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

class Gtk3DrawingAreaPainter : public Painter
{
public:
	//Constructor
	inline Gtk3DrawingAreaPainter(cairo_t* cr) : cr(cr)
	{
	}

	//Methods
	void BeginPath() override
	{
		cairo_new_path(this->cr);
	}

	void BezierTo(const Math::Vector2D &refCP0, const Math::Vector2D &refCP1, const Math::Vector2D &refTo) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	void ClosePath() override
	{
		cairo_close_path(this->cr);
	}

	Math::SizeD ComputeTextSize(const String &text) const override
	{
		cairo_text_extents_t te;
		cairo_text_extents (this->cr, (const char *)text.ToUTF8().GetRawZeroTerminatedData(), &te);

		return {te.width, te.height};
	}

	void DrawText(const Math::PointD &p, const String &text) override
	{
		cairo_move_to (this->cr, p.x, p.y);
		cairo_show_text(this->cr, (const char *)text.ToUTF8().GetRawZeroTerminatedData());
	}

	void Fill() override
	{
		cairo_set_source_rgba(this->cr, this->fillColor.r, this->fillColor.g, this->fillColor.b, this->fillColor.a);
		cairo_fill_preserve(this->cr);
	}

	void LineTo(const Math::Vector2D& pos) override
	{
		cairo_line_to(this->cr, pos.x, pos.y);
	}

	void MoveTo(const Math::Vector2D& pos) override
	{
		cairo_move_to(this->cr, pos.x, pos.y);
	}

	void SetFillColor(const Color &color) override
	{
		this->fillColor = color;
	}

	void SetStrokeColor(const Color& color) override
	{
		this->strokeColor = color;
	}

	void SetStrokeWidth(float64 strokeWidth) override
	{
		cairo_set_line_width(this->cr, strokeWidth);
	}

	void Stroke() override
	{
		cairo_set_source_rgba(this->cr, this->strokeColor.r, this->strokeColor.g, this->strokeColor.b, this->strokeColor.a);
		cairo_stroke_preserve(this->cr);
	}

private:
	//State
	cairo_t* cr;
	Color fillColor;
	Color strokeColor;
};

class YAxisInvertedPainter : public Painter
{
public:
	//Constructor
	inline YAxisInvertedPainter(UniquePointer<Painter>&& internalPainter, float64 height) : internalPainter(Move(internalPainter)), height(height)
	{
	}

	void BeginPath() override
	{
		this->internalPainter->BeginPath();
	}

	void BezierTo(const Math::Vector2D &refCP0, const Math::Vector2D &refCP1, const Math::Vector2D &refTo) override
	{
		this->internalPainter->BezierTo(this->Transform(refCP0), this->Transform(refCP1), this->Transform(refTo));
	}

	void ClosePath() override
	{
		this->internalPainter->ClosePath();
	}

	Math::SizeD ComputeTextSize(const String &text) const override
	{
		return this->internalPainter->ComputeTextSize(text);
	}

	void DrawText(const Math::PointD &p, const String &text) override
	{
		this->internalPainter->DrawText(this->Transform(p), text);
	}

	void Fill() override
	{
		this->internalPainter->Fill();
	}

	void LineTo(const Math::Vector2D &pos) override
	{
		this->internalPainter->LineTo(this->Transform(pos));
	}

	void MoveTo(const Math::Vector2D &pos) override
	{
		this->internalPainter->MoveTo(this->Transform(pos));
	}

	void SetFillColor(const Color &color) override
	{
		this->internalPainter->SetFillColor(color);
	}

	void SetStrokeColor(const Color &color) override
	{
		this->internalPainter->SetStrokeColor(color);
	}

	void SetStrokeWidth(float64 strokeWidth) override
	{
		this->internalPainter->SetStrokeWidth(strokeWidth);
	}

	void Stroke() override
	{
		this->internalPainter->Stroke();
	}

private:
	//State
	UniquePointer<Painter> internalPainter;
	float64 height;

	//Inline
	inline Math::PointD Transform(const Math::PointD& v)
	{
		return {v.x, this->height - v.y};
	}

	inline Math::Vector2D Transform(const Math::Vector2D& v)
	{
		return {v.x, this->height - v.y};
	}
};

//Local functions
static gboolean ButtonSlot(GtkWidget* gtkWidget, GdkEvent* event, gpointer user_data)
{
	Widget& widget = *(Widget*)user_data;

	MouseButton button;
	switch(event->button.button)
	{
		case 1:
			button = MouseButton::Left;
			break;
		case 3:
			button = MouseButton::Right;
			break;
		default:
			return FALSE;
	}

	switch(event->button.type)
	{
		case GDK_BUTTON_PRESS:
		case GDK_BUTTON_RELEASE:
		{
			bool isDown = event->button.type == GDK_BUTTON_PRESS;
			Math::PointD point = {event->button.x, widget.GetSize().height - event->button.y};
			KeyboardModifiers keyboardModifiers;

			MouseClickEvent mce(button, isDown, point, keyboardModifiers);
			widget.Event(mce);

			return mce.WasAccepted();
		}
	}

	return FALSE;
}

static bool DrawSlot(GtkWidget* gtkWidget, cairo_t* cr, gpointer user_data)
{
	Widget& widget = *(Widget*)user_data;

	GtkAllocation gtkAllocation;
	gtk_widget_get_allocation(GTK_WIDGET(gtkWidget), &gtkAllocation);

	PaintEvent paintEvent({ static_cast<float64>(gtkAllocation.x), static_cast<float64>(gtkAllocation.y),
							static_cast<float64>(gtkAllocation.width), static_cast<float64>(gtkAllocation.height) });
	paintEvent.osHandle.handle = cr;
	widget.Event(paintEvent);

	return paintEvent.WasAccepted();
}

static bool MouseMotionSlot(GtkWidget *gtkWidget, GdkEventMotion *event, gpointer user_data)
{
	Widget& widget = *(Widget*)user_data;

	Math::PointD pos = {event->x, ((float64)widget.GetSize().height) - event->y};
	KeyboardModifiers keyboardModifiers;
	MouseEvent mouseEvent(EventType::MouseMoved, pos, keyboardModifiers);

	widget.Event(mouseEvent);

	return mouseEvent.WasAccepted();
}

//Constructor
Gtk3DrawingAreaBackend::Gtk3DrawingAreaBackend(UIBackend &uiBackend, Widget &widget) : DrawableWidgetBackend(uiBackend, widget), Gtk3WidgetBackend(uiBackend, gtk_drawing_area_new()), WidgetBackend(uiBackend)
{
	GtkWidget* gtkWidget = this->GetGtkWidget();
	gtk_widget_add_events(gtkWidget, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_SCROLL_MASK);
	g_signal_connect(gtkWidget, "draw", G_CALLBACK(DrawSlot), &widget);
	g_signal_connect(gtkWidget, "button-press-event", G_CALLBACK(ButtonSlot), &widget);
	g_signal_connect(gtkWidget, "button-release-event", G_CALLBACK(ButtonSlot), &widget);
	g_signal_connect(gtkWidget, "motion-notify-event", G_CALLBACK(MouseMotionSlot), &widget);
}

//Public methods
UniquePointer<Painter> Gtk3DrawingAreaBackend::CreatePainter(const OSHandle& osHandle)
{
	auto gtkPainter = new Gtk3DrawingAreaPainter((cairo_t*)osHandle.handle);

	float64 height = gtk_widget_get_allocated_height(this->GetGtkWidget());
	return new YAxisInvertedPainter(gtkPainter, height);
}

void Gtk3DrawingAreaBackend::Repaint()
{
	gtk_widget_queue_draw(this->GetGtkWidget());
}