#pragma once

#include "engine.hpp"

#include "console.hpp"

class app : public application {
    void on_startup() {
        graphics->init();

        input->handler(platform::input::POINTER, platform::input::DOWN, [](const platform::input::event& ev) { scene::global().mouse_click(ev); }, 1);
        input->handler(platform::input::POINTER, platform::input::DRAG, [](const platform::input::event& ev) { scene::global().mouse_drag(ev); }, 1);
        input->handler(platform::input::POINTER, platform::input::RELEASE, [](const platform::input::event& ev) { scene::global().mouse_release(ev); }, 1);

        input->handler(platform::input::KEY, platform::input::DOWN, [](const platform::input::event& ev) { scene::global().keyboard_input(ev); }, 0);
        input->handler(platform::input::KEY, platform::input::UP, [](const platform::input::event& ev) { scene::global().keyboard_input(ev); }, 0);

        scene::global().add("debug", new console());
        
        // Just to avoid having to do keyboard input on ios for now
        scene::global().toggle("debug");

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

        gui->draw();
        graphics->flush();
    }

    void on_interval() {
        scene::global().run();
    }
};
