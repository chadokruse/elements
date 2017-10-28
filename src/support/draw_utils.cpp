/*=============================================================================
   Copyright (c) 2016-2017 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <photon/support/draw_utils.hpp>

namespace photon
{
   void draw_box_vgradient(canvas& cnv, rect bounds, float corner_radius)
   {
      auto gradient = canvas::linear_gradient{
         bounds.top_left(),
         bounds.bottom_left()
      };

      gradient.add_color_stop({ 0.0, { 255, 255, 255, 16 } });
      gradient.add_color_stop({ 0.8, { 0, 0, 0, 16 } });
      cnv.fill_style(gradient);

      cnv.begin_path();
      cnv.round_rect(bounds, corner_radius);
      cnv.fill();

      cnv.begin_path();
      cnv.move_to(point{ bounds.left+0.5f, bounds.bottom-0.5f });
      cnv.line_to(point{ bounds.right-0.5f, bounds.bottom-0.5f });
      cnv.stroke_style(color{ 0, 0, 0, 32 });
      cnv.line_width(1);
      cnv.stroke();
   }

   void draw_panel(canvas& cnv, rect bounds, color c, float corner_radius)
   {
      rect const shadow_offset  = { -10, -10, +20, +30 };

      // Panel fill
      cnv.begin_path();
      cnv.round_rect(bounds, corner_radius);
      cnv.fill_style(c);
      cnv.fill();

      // Simulated blurred shadow. cairo does not have blur yet :-(
      {
         auto save = cnv.new_state();

         cnv.begin_path();
         cnv.rect(bounds.inset(-100, -100));
         cnv.round_rect(bounds.inset(0.5, 0.5), corner_radius);
         cnv.fill_rule(canvas::fill_odd_even);
         cnv.clip();

         rect shr = bounds;
         shr.left -= 2;
         shr.top -= 2;
         shr.right += 6;
         shr.bottom += 6;

         cnv.begin_path();
         cnv.round_rect(shr, corner_radius*2);
         cnv.fill_style(color(0, 0, 0, 20));
         cnv.fill();

         shr.left += 1;
         shr.top += 1;
         shr.right -= 2;
         shr.bottom -= 2;
         cnv.begin_path();
         cnv.round_rect(shr, corner_radius*1.5);
         cnv.fill_style(color(0, 0, 0, 30));
         cnv.fill();

         shr.left += 1;
         shr.top += 1;
         shr.right -= 2;
         shr.bottom -= 2;
         cnv.begin_path();
         cnv.round_rect(shr, corner_radius);
         cnv.fill_style(color(0, 0, 0, 40));
         cnv.fill();
      }
   }

   void draw_button(canvas& cnv, rect bounds, color c, float corner_radius)
   {
      auto gradient = canvas::linear_gradient{
         bounds.top_left(),
         bounds.bottom_left()
      };

      gradient.add_color_stop({ 0.0, { 255, 255, 255, 32 } });
      gradient.add_color_stop({ 1.0, { 0, 0, 0, 32 } });
      cnv.fill_style(gradient);

      cnv.begin_path();
      cnv.round_rect(bounds.inset(1, 1), corner_radius-1);
      cnv.fill_style(c);
      cnv.fill();
      cnv.round_rect(bounds.inset(1, 1), corner_radius-1);
      cnv.fill_style(gradient);
      cnv.fill();

      cnv.begin_path();
      cnv.round_rect(bounds.inset(0.5, 0.5), corner_radius-0.5);
      cnv.stroke_style(color{ 0, 0, 0, 48 });
      cnv.stroke();
   }

   void draw_knob(canvas& cnv, circle cp, color c)
   {
      auto state = cnv.new_state();
      float radius = cp.radius * 0.85;
      float inset = cp.radius * 0.15;

      // Draw beveled knob
      {
         auto gradient = canvas::radial_gradient{
            { cp.cx, cp.cy }, radius*0.75f,
            { cp.cx, cp.cy }, radius
         };

         gradient.add_color_stop({ 0.0, c });
         gradient.add_color_stop({ 0.5, c.opacity(0.5) });
         gradient.add_color_stop({ 1.0, c.level(0.5).opacity(0.5) });

         cnv.fill_style(gradient);
         cnv.begin_path();
         cnv.circle(cp.inset(inset));
         cnv.fill();
      }

      // Draw some 3D highlight
      {
         auto hcp = cp.center().move(-radius, -radius);
         auto gradient = canvas::radial_gradient{
            hcp, radius*0.5f,
            hcp, radius*2
         };

         gradient.add_color_stop({ 0.0, { 1.0, 1.0, 1.0, 0.4 } });
         gradient.add_color_stop({ 1.0, { 0.6, 0.6, 0.6, 0.0 } });

         cnv.fill_style(gradient);
         cnv.begin_path();
         cnv.circle(cp.inset(inset));
         cnv.fill();
      }

      // Draw the outline
      {
         cnv.stroke_style(colors::black.opacity(0.1));
         cnv.circle(cp.inset(inset));
         cnv.line_width(radius/30);
         cnv.stroke();
      }

      // Draw knob rim
      {
         cnv.begin_path();
         cnv.circle(cp);
         cnv.circle(cp.inset(inset));
         cnv.fill_rule(canvas::fill_odd_even);
         cnv.clip();

         auto bounds = cp.bounds();
         auto gradient = canvas::linear_gradient{
            bounds.top_left(),
            bounds.bottom_left()
         };

         gradient.add_color_stop({ 1.0, { 255, 255, 255, 64 } });
         gradient.add_color_stop({ 0.0, { 0, 0, 0, 32 } });
         cnv.fill_style(gradient);

         cnv.begin_path();
         cnv.rect(bounds);
         cnv.fill_style(gradient);
         cnv.fill();
      }
   }

   void draw_indicator(canvas& cnv, rect bounds, color c)
   {
      cnv.fill_style(c);
      cnv.begin_path();
      cnv.round_rect(bounds, bounds.height()/12);
      cnv.fill();
   }
}
