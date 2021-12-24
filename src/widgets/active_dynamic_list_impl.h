#ifndef ACTIVE_DYNAMIC_LIST_IMPL_H
#define ACTIVE_DYNAMIC_LIST_IMPL_H

/*=============================================================================
   Copyright (c) 2021 Johann Philippe

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <widgets/active_dynamic_list.h>
#include <elements/view.hpp>
#include<iostream>


template<typename T>
view_limits active_dynamic_list<T>::limits(basic_context const& ctx) const
{
    if (_composer)
    {
        if (_update_request)
            update(ctx);
        auto w_limits = _composer->width_limits(ctx);
        if (_composer->size())
        {
           return {
              { w_limits.min, float(_height) }
            , { w_limits.max, float(_height) }
           };
        }
    }
    return {{ 0, 0 }, { 0, 0 }};
}

   template<typename T>
   void active_dynamic_list<T>::draw(context const& ctx)
   {
      auto& cnv = ctx.canvas;
      auto  state = cnv.new_state();
      auto  clip_extent = cnv.clip_extent();
      auto  top = ctx.bounds.top;

      if (!intersects(ctx.bounds, clip_extent))
         return;

      auto it = std::lower_bound(_rows.begin(), _rows.end(),
         clip_extent.top-top,
         [](auto const& row, double pivot)
         {
            return (row.pos + row.height) < pivot;
         }
      );

      // Draw the rows within the visible bounds of the view
      std::size_t new_start = it - _rows.begin();
      for (; it != _rows.end(); ++it)
      {
         auto& row = *it;
         context rctx { ctx, row.elem_ptr.get(), ctx.bounds };
         rctx.bounds.top = top + row.pos;
         rctx.bounds.height(row.height);
         if (intersects(clip_extent, rctx.bounds))
         {
            if (!row.elem_ptr)
            {
               row.elem_ptr = _composer->compose(it-_rows.begin());
               row.elem_ptr->layout(rctx);
               row.layout_id = _layout_id;
            }
            else if (row.layout_id != _layout_id)
            {
               row.elem_ptr->layout(rctx);
               row.layout_id = _layout_id;
            }
            row.elem_ptr->draw(rctx);
         }
         if (rctx.bounds.top > clip_extent.bottom)
            break;
      }
      std::size_t new_end = it - _rows.begin();

      // Cleanup old rows
      if (new_start != _previous_window_start || new_end != _previous_window_end)
      {
         for (auto i = _previous_window_start; i != _previous_window_end; ++i)
         {
            if (i < new_start || i >= new_end)
            {
               _rows[i].elem_ptr.reset();
               _rows[i].layout_id = -1;
            }
         }
      }

      _previous_window_start = new_start;
      _previous_window_end = new_end;
      _previous_size.x = ctx.bounds.width();
      _previous_size.y = ctx.bounds.height();
   }

   template<typename T>
   void active_dynamic_list<T>::layout(context const& ctx)
   {
      if (_previous_size.x != ctx.bounds.width() ||
         _previous_size.y != ctx.bounds.height())
      {
         _previous_size.x = ctx.bounds.width();
         _previous_size.y = ctx.bounds.height();
         ++_layout_id;
      }
   }

   template<typename T>
   void active_dynamic_list<T>::update()
   {
      _update_request = true;
      _rows.clear();
      _height = 0;
   }

   template<typename T>
   void active_dynamic_list<T>::update(basic_context const& ctx) const
   {
      if (_composer)
      {
         if (auto size = _composer->size())
         {
            double y = 0;
            _rows.reserve(_composer->size());
            for (std::size_t i = 0; i != size; ++i)
            {
               auto line_height = _composer->line_height(i, ctx);
               _rows.push_back({ y, line_height, nullptr });
               y += line_height;
            }
            _height = y;
         }
      }
      ++_layout_id;
      _update_request = false;
   }

   // MY CLICK

   template<typename T>
      bool active_dynamic_list<T>::click(const context &ctx, mouse_button btn)
      {
          std::cout << "list click " << std::endl;
          size_t i = 0;
          for(auto & it : _rows)
          {
              context rctx{ctx, it.elem_ptr.get(), ctx.bounds };
              rctx.bounds.top = ctx.bounds.top + it.pos;
              rctx.bounds.height(it.height);
              if(rctx.bounds.includes(btn.pos))
              {
                  std::cout << "at index list : " << i << std::endl;
                  new_focus(rctx, i);
                  return it.elem_ptr->click(rctx, btn);
              }
              i++;
          }
          new_focus(ctx, _focus);
          return false;
      }

   template<typename T>
   bool active_dynamic_list<T>::text(context const& ctx, text_info info)
   {
      if (_focus != -1)
      {
         rect  bounds = bounds_of(ctx, _focus);
         auto& focus_ = *_rows[_focus].elem_ptr;
         context ectx{ ctx, &focus_, bounds };
         return focus_.text(ectx, info);
      };

      return false;
   }


   template<typename T>
      void active_dynamic_list<T>::new_focus(context const& ctx, int index)
      {
          if (_focus != -1 )
          {
             _composer->compose(_focus)->end_focus();
             ctx.view.refresh(ctx);
          }

          // start a new focus
          _focus = index;
          if (_focus != -1 && _rows[_focus].elem_ptr != nullptr)
          {
             _rows[_focus].elem_ptr->begin_focus();
             ctx.view.refresh(ctx);
          }
      }

   template<typename T>
      rect active_dynamic_list<T>::bounds_of(context const& ctx, int ix) const
      {
              rect r = ctx.bounds;
              r.top = ctx.bounds.top + _rows[ix].pos;
              r.height(_rows[ix].height);
              return r;
      }

   template<typename T>
      bool active_dynamic_list<T>::key(const context &ctx, key_info k)
      {

          auto&& try_key = [&](int ix) -> bool
          {
             rect bounds = bounds_of(ctx, ix);
             auto& e = *_rows[ix].elem_ptr;
             context ectx{ ctx, &e, bounds };
             bool b = e.key(ectx, k);
             return b;
          };

          auto&& try_focus = [&](int ix) -> bool
          {
             if (_rows[ix].elem_ptr->wants_focus())
             {
                new_focus(ctx, ix);
                return true;
             }
             return false;
          };

          if (_focus != -1)
          {
             if (try_key(_focus))
                return true;
          }

          if ((k.action == key_action::press || k.action == key_action::repeat)
             && k.key == key_code::tab && _rows.size())
          {
             int next_focus = _focus;
             //bool reverse = (k.modifiers & mod_shift) ^ reverse_index();
             if(next_focus == -1)
             {
                while (++next_focus != static_cast<int>(_rows.size()))
                   if (try_focus(next_focus))
                      return true;
                return false;
             }
             else
             {
                while (--next_focus >= 0)
                   if (_rows[next_focus].elem_ptr->wants_focus())
                      if (try_focus(next_focus))
                         return true;
                return false;
             }
          }
          return false;
      }


   template<typename T>
      bool active_dynamic_list<T>::wants_focus() const
      {
         for (std::size_t ix = 0; ix != _rows.size(); ++ix)
            if (_rows[ix].elem_ptr != nullptr && _rows[ix].elem_ptr->wants_focus())
               return true;
         return false;
      }

   template<typename T>
      void active_dynamic_list<T>::begin_focus()
      {
         if (_focus == -1)
            _focus = _saved_focus;
         if (_focus == -1)
         {
            for (std::size_t ix = 0; ix != _rows.size(); ++ix)
               if (_rows[ix].elem_ptr != nullptr && _rows[ix].elem_ptr->wants_focus())
               {
                  _focus = ix;
                  break;
               }
         }
         if (_focus != -1 && _rows[_focus].elem_ptr != nullptr)
            _rows[_focus].elem_ptr->begin_focus();
      }

   template<typename T>
      void active_dynamic_list<T>::end_focus()
      {
         if (_focus != -1 && _rows[_focus].elem_ptr != nullptr)
            _rows[_focus].elem_ptr->end_focus();
         _saved_focus = _focus;
         _focus = -1;
      }

   template<typename T>
      element const* active_dynamic_list<T>::focus() const
      {
         return (_rows.empty() || (_focus == -1))? 0 : _rows[_focus].elem_ptr.get();
      }

   template<typename T>
      element* active_dynamic_list<T>::focus()
      {
         return (_rows.empty() || (_focus == -1))? 0 : _rows[_focus].elem_ptr.get();
      }

   template<typename T>
      void active_dynamic_list<T>::focus(std::size_t index)
      {
         if (index < _rows.size())
            _focus = int(index);
      }


   template<typename T>
      void active_dynamic_list<T>::reset()
      {
         _focus = -1;
         _saved_focus = -1;
         _click_tracking = -1;
         _cursor_tracking = -1;
         _cursor_hovering.clear();
      }

   template<typename T>
      void active_dynamic_list<T>::resize(size_t n)
      {
          this->_composer->resize(n);
          this->update();
      }

#endif // ACTIVE_DYNAMIC_LIST_IMPL_H
