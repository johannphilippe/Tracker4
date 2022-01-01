#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H
#include<elements.hpp>
#include<tracker/jtracker.h>
#include<dialogs/elements_dialog_base.h>
#include<widgets/spinbox.h>
#include<widgets/custom_container.h>
using namespace cycfi::elements;

class click_callback
{
public:
    std::function<void(element*)> click_remove = [](element* ptr) {};
};

class option_item :
        public click_callback,
        public audio_cs_option,
        public array_composite<2, htile_element>
{
public:
    auto make_view();
    option_item(std::string name, bool enable);

    bool _selected = false;
};

class cs_option_list : public vtile_composite
{
public:
    cs_option_list()
        : vtile_composite()
    {
        for(auto & it : jtracker::data.audio_config.additional_options)
        {
            add_item(it.option, it.enabled);
        }
    }

    void add_item(std::string s, bool enabled);
    void add_item();

    void remove_selected();

    void pop();

    int _selected = -1;
    std::vector<std::shared_ptr<option_item>> items;
};

class audio_options_view
{
public:
    audio_options_view()
        : sample_rate_spin(spinbox_list_controller<size_t>(
                               jtracker::data.audio_config.sample_rate,
                               std::vector<size_t>(
                                   audio_rate_list,
                                   audio_rate_list + audio_rate_list_size)))
        , ksmps_spin(spin_controller<size_t>(1, 22'050,
                                             jtracker::data.audio_config.ksmps, 1))
        , dbfs_spin(spin_controller<size_t>(1, 64'000,
                                            jtracker::data.audio_config.dbfs, 1))
        , inchnls_spin(spin_controller<int>(0, std::numeric_limits<int>::max(),
                                            jtracker::data.audio_config.i_nchnls, 1))
        , nchnls_spin(spin_controller<int>(0, std::numeric_limits<int>::max(),
                                           jtracker::data.audio_config.nchnls, 1))
    {}

    auto make_view();

    spinbox_list<size_t> sample_rate_spin;
    spinbox<size_t> ksmps_spin, dbfs_spin;
    spinbox<int> inchnls_spin, nchnls_spin;

    cs_option_list options_list;
};

class options_dialog : public elements_dialog_base
{
public:
    static options_dialog* get_instance();

    auto make_options_tab();
    void update_config();

    void make_content() override;

private:
    options_dialog() {};
    virtual ~options_dialog() {};
    options_dialog& operator= (const options_dialog&) = delete;
    options_dialog(const options_dialog& ) = delete;
    static options_dialog *instance;

    // members
    std::shared_ptr<audio_options_view> audio_options;

};




#endif // OPTIONS_DIALOG_H
