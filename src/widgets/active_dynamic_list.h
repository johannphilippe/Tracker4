#ifndef ACTIVE_DYNAMIC_LIST_H
#define ACTIVE_DYNAMIC_LIST_H

#include<elements.hpp>
#include<iostream>
#include<widgets/active_cell_composer.h>

using namespace cycfi::elements;

////////////////////////////////////////////////////////////////////////////
// An active dynamic_list class
////////////////////////////////////////////////////////////////////////////
template<typename T>
class active_dynamic_list : public element
{
public:

   using composer_ptr = std::shared_ptr<active_cell_composer>;

                              active_dynamic_list(composer_ptr composer)
                               : _composer(composer)
                              {}

   view_limits                limits(basic_context const& ctx) const override;
   void                       draw(context const& ctx) override;
   void                       layout(context const& ctx) override;

   void                       update();
   void                       update(basic_context const& ctx) const;

   bool click(const context &ctx, mouse_button btn) override;
   bool text(context const& ctx, text_info info) override;
   bool key(const context &ctx, key_info k) override;
   void new_focus(context const& ctx, int index);

   bool                    wants_focus() const override;
   void                    begin_focus() override;
   void                    end_focus() override;
   element const*          focus() const override;
   element*                focus() override;
   void                    focus(std::size_t index);
   virtual void            reset();

   rect bounds_of(context const& ctx, int ix) const;

   composer_ptr get_composer() {return _composer;}


   void resize(size_t n);
   // to resize and associating callbacks
   void resize(size_t n , std::function<void(std::function<void(void)>)> f);
private:

   struct row_info
   {
      double                  pos;
      double                  height;
      element_ptr             elem_ptr;
      int                     layout_id = -1;
   };

   using rows_vector = std::vector<row_info>;

   composer_ptr               _composer;
   point                      _previous_size;
   std::size_t                _previous_window_start = 0;
   std::size_t                _previous_window_end = 0;

   mutable rows_vector        _rows;
   mutable double             _height = 0;
   mutable int                _layout_id = 0;
   mutable bool               _update_request = true;

   int 					   _focus = -1;
   int 					   _saved_focus = -1;
   int                     _click_tracking = -1;
   int                     _cursor_tracking = -1;;
   std::set<int>           _cursor_hovering;

};

#include<widgets/active_dynamic_list_impl.h>


#endif // ACTIVE_DYNAMIC_LIST_H
