#pragma once

//#include "engine.hpp"

//#include "console.hpp"

class app : public application {

    spatial::position pos;

    void on_startup() {
        scene::global().callback("position", [this](scene::parameters_t p)->value_t {
            if (scene::global().has("position")) {
                this->pos.reposition(std::get<spatial::vector>(scene::global().get("position")));
            }
            return this->pos.eye;
            });
        scene::global().callback("ambient.position", [this](scene::parameters_t p)->value_t {
            if (scene::global().has("ambient.position")) {
                graphics->ambient.position.reposition(std::get<spatial::vector>(scene::global().get("ambient.position")));
            }
            return  graphics->ambient.position.eye;
            });
        scene::global().callback("ambient.color", [this](scene::parameters_t p)->value_t {
            if (scene::global().has("ambient.color")) {
                graphics->ambient.color = std::get<spatial::vector>(scene::global().get("ambient.color"));
            }
            return graphics->ambient.color;
            });
        scene::global().callback("ambient.bias", [this](scene::parameters_t p)->value_t {
            if (scene::global().has("ambient.bias")) {
                graphics->ambient.bias = std::get<double>(scene::global().get("ambient.bias"));
            }
            return graphics->ambient.bias;
            });
        scene::global().callback("ambient.strength", [this](scene::parameters_t p)->value_t {
            if (scene::global().has("ambient.strength")) {
                graphics->ambient.strength = std::get<double>(scene::global().get("ambient.strength"));
            }
            return graphics->ambient.strength;
            });

        graphics->init();

        input->handler(platform::input::POINTER, platform::input::DOWN, [](const platform::input::event& ev) { scene::global().mouse_click(ev); }, 1);
        input->handler(platform::input::POINTER, platform::input::DRAG, [](const platform::input::event& ev) { scene::global().mouse_drag(ev); }, 1);
        input->handler(platform::input::POINTER, platform::input::RELEASE, [](const platform::input::event& ev) { scene::global().mouse_release(ev); }, 1);

        input->handler(platform::input::KEY, platform::input::DOWN, [](const platform::input::event& ev) { scene::global().keyboard_input(ev); }, 0);
        input->handler(platform::input::KEY, platform::input::UP, [](const platform::input::event& ev) { scene::global().keyboard_input(ev); }, 0);

        //scene::global().add("debug", new console());
        
        // Just to avoid having to do keyboard input on ios for now
        scene::global().toggle("debug");

        //scene::global().call("/load entity objects/suzanne object");
        scene::global().call("/set position (0.0,-1.0,-2.0)");
        scene::global().call("/set rotation 0.2");

        scene::global().call("/set ambient.position (1.0,1.0,1.0)");
        scene::global().call("/set ambient.color (1.0,1.0,1.0)");
        scene::global().call("/set ambient.bias 0.2");
        scene::global().call("/set ambient.strength 1.0");

        //scene::global().call("/load shader shaders/basic shader");
        //scene::global().call("/compile");
        scene::global().call("/load shader basic shader");
        scene::global().call("/load entity objects/suzanne object");
        scene::global().call("/compile");

        gui->position();
    }

    void on_resize() {
        graphics->dimensions(width, height);

        graphics->projection(90);

        scene::global().dimensions(width, height);

        gui->projection = graphics->ortho;

        gui->position();
    }

    void on_draw() {
        graphics->clear();

        scene::global().draw();

        if (assets->has<type::program>("shader")) {
            graphics->draw(*assets->get<type::entity>("object").object, assets->get<type::program>("shader"), graphics->perspective, pos);
        }

        //gui->draw();
        graphics->flush();
    }

    void on_interval() {
        scene::global().run();
        
        if (assets->has<type::program>("shader") == false) {
            return;
        }

        pos.spin(std::get<double>(scene::global().get("rotation")));
        return;
        auto& shader = assets->get<type::program>("shader");
        if (shader.compiled() == false) {
            graphics->compile(shader);
            scene::global().call("/compile");

            validate(shader, "a_Vertex", shader.a_Vertex);
            validate(shader, "a_Texture", shader.a_Texture);
            validate(shader, "a_Normal", shader.a_Normal);
            validate(shader, "a_ModelMatrix", shader.a_ModelMatrix);
            validate(shader, "a_Identifier", shader.a_Identifier);
            validate(shader, "a_Flags", shader.a_Flags);
            validate(shader, "u_Clipping", shader.u_Clipping);
            validate(shader, "u_ProjectionMatrix", shader.u_ProjectionMatrix);
            validate(shader, "u_ViewMatrix", shader.u_ViewMatrix);
            validate(shader, "u_ModelMatrix", shader.u_ModelMatrix);
            validate(shader, "u_LightingMatrix", shader.u_LightingMatrix);
            validate(shader, "u_SurfaceTextureUnit", shader.u_SurfaceTextureUnit);
            validate(shader, "u_ShadowTextureUnit", shader.u_ShadowTextureUnit);
            validate(shader, "u_NormalTextureUnit", shader.u_NormalTextureUnit);
            validate(shader, "u_DepthTextureUnit", shader.u_DepthTextureUnit);
            validate(shader, "u_BlurTextureUnit", shader.u_BlurTextureUnit);
            validate(shader, "u_PickingTextureUnit", shader.u_PickingTextureUnit);
            validate(shader, "u_AmbientLightPosition", shader.u_AmbientLightPosition);
            validate(shader, "u_AmbientLightColor", shader.u_AmbientLightColor);
            validate(shader, "u_AmbientLightBias", shader.u_AmbientLightBias);
            validate(shader, "u_AmbientLightStrength", shader.u_AmbientLightStrength);
            validate(shader, "u_RelativeParticles", shader.u_RelativeParticles);
            validate(shader, "u_Flags", shader.u_Flags);
            validate(shader, "u_Parameters", shader.u_Parameters);
        }
    }

    bool uses(std::string text, std::string parameter) {
        auto pos = text.find("void main()");
        while (pos != std::string::npos) {
            bool found = (pos = text.find(parameter, pos)) != std::string::npos;
            if (found) {
                auto before = text[pos - 1];
                auto after = text[pos + parameter.length()];
                if (pos != 0 && isalpha(before)) {
                    found = false;
                }
                if (isalpha(after)) {
                    found = false;
                }
            }
            if (found) {
                return true;
            }
        }
        return false;
    }

    std::string definition(std::string text, std::string parameter) {
        auto start = text.find("void main()");
        auto pos = text.find(" " + parameter + ";");
        if (pos != std::string::npos && pos < start) {
            auto prior = text.rfind("\n", pos);
            prior = prior == std::string::npos ? 0 : prior + 1;
            return text.substr(prior, pos - prior + parameter.length() + 2);
        }
        return "";
    }

    bool validate(type::program& shader, std::string parameter, unsigned int location) {
        bool missing = false;
        if (location == -1 && (uses(shader.vertex.text, parameter) || uses(shader.fragment.text, parameter))) {
            auto vertex_define = definition(shader.vertex.text, parameter);
            auto fragment_define = definition(shader.fragment.text, parameter);

            if (vertex_define.empty() == false) {
                //console::trace() << vertex_define << " *** vertex location missing ***";
            }
            else if (fragment_define.empty() == false) {
                //console::trace() << fragment_define << " *** fragment location missing ***";
            }
            else {
                //console::trace() << vertex_define << " *** not defined ***";
            }

            missing = true;
        }
        return !missing;
    }
};
