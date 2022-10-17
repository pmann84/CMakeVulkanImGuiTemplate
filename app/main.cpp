#include <application.hpp>
#include <logging.hpp>

class my_app : public application
{
public:
    my_app(application_data props) : application(props)
    {
    }

    // This function is run every frame
    void on_update(float ts) override
    {
        logging::debug("Updating my app: timestep = {0}", ts);
    }

    void on_ui_update() override
    {
        logging::debug("Updating ui");
    }
};

int main()
{
    application_data props{ "My Amazing App", 1024, 768 };
    try 
    {
        my_app app(props);
        app.run();
    }
    catch (const std::exception& e)
    {
        logging::critical("Error running application: {0}", e.what());
    }
    return 0;
}