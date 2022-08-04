#pragma once

class console : public scene::handler {
public:
    class trace {
    public:
        trace() {}
        ~trace() {
            scene::debug().content.add(ss.str());
        }

        trace& operator << (const std::string& v) {
            ss << v;
            return *this;
        }
        trace& operator << (size_t v) {
            ss << v;
            return *this;
        }
        trace& operator << (int v) {
            ss << v;
            return *this;
        }
        trace& operator << (unsigned int v) {
            ss << v;
            return *this;
        }
        trace& operator << (float v) {
            ss << v;
            return *this;
        }
        trace& operator << (double v) {
            ss << v;
            return *this;
        }

    protected:
        std::stringstream ss;
    };

    bool load() {
        scene::global().call("/load shader shaders/basic gui");
        scene::global().call("/load font fonts/consolas-22 default");
        scene::global().call("/compile");

        gui->shader = assets->get<type::program>("gui");
        gui->font = assets->get<type::font>("default");

        gui->create(&scene::debug(), 512, 720, 0, 0, 0, 80)->position(graphics->width() - 512 - 20, 20);
        scene::debug().selectable = true;
        scene::debug().alignment = platform::interface::widget::positioning::bottom;

        scene::debug().events.handler(platform::input::POINTER, platform::input::WHEEL, [](const platform::input::event& ev) {
            // TODO: scroll up and down the contents
            }, 0);

        gui->create(&commandline, 512, 20, 0, 0, 0, 80)->position(graphics->width() - 512 - 20, 750);
        commandline.selectable = true;
        commandline.input = true;
        commandline.multiline = false;
        commandline.content.limit = 1; // TODO: This should be covered by the multiline flag, but currently isn't.
        commandline.events.handler(platform::input::KEY, platform::input::DOWN, [this](const platform::input::event& ev) {
            std::vector<std::string> content;
            switch (ev.identifier) {
            case(27):
                gui->select(NULL);
            case(38):
                commandline.content.position(1);
                break;
            case(40):
                commandline.content.position(-1);
                break;
            case(8): // Backspace to remove a character
                commandline.content.truncate(1);
                break;
            case(13): // Enter to submit
                content = commandline.content.get();
                if (content.size()) {
                    if (content.back().size() && content.back()[0] == '/') {
                        scene::global().call(content.back());
                    }
                    else {
                        trace() << content.back();
                    }
                    if (commandline.content.position() == 0) {
                        commandline.content.add();
                    }
                    commandline.content.position(content.back().size() * -1);
                }
                break;
            default: // Every other printable gets added to the contents
                commandline.content.append(input->printable(ev.identifier));
            };
            });

        // Start off with an empty command
        commandline.content.add();

        return true;
    }

    void start() {
        scene::debug().visible = true;
        commandline.visible = true;
        gui->select(&commandline);
    }

    void stop() {
        gui->select(NULL);
        scene::debug().visible = false;
        commandline.visible = false;
    }

    platform::interface::textbox commandline;
};
