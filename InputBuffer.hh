#ifndef LIGHTNING3D_INPUT_BUFFER_HH_
#define LIGHTNING3D_INPUT_BUFFER_HH_

#include "ApplicationMessageHandler.hh"

#include <chrono>
#include <deque>
#include <variant>

namespace Lightning3D {
    struct KeyboardEvent {
        enum struct Type {
            Press,
            Release
        };

        Key key;
        Type type;
    };

    struct MouseEvent {
        enum struct Type {
            Press,
            Release
        };

        MouseButton btn;
        Type type;
    };

    struct InputEvent {
        std::variant<KeyboardEvent, MouseEvent> evt;

        std::chrono::system_clock::time_point stamp;
    };

    class InputBuffer {
    public:
        InputBuffer() {

        }

        ~InputBuffer() {

        }

        auto PushEvent(InputEvent evt) -> void {
            m_inputEvents.push_back(evt);
        }

        auto KeyPress(Key key, std::chrono::microseconds duration = std::chrono::microseconds(0)) -> InputBuffer & {
            for (auto it = m_inputEvents.cbegin(); it != m_inputEvents.cend(); ++it) {
                if (auto keyboardEvt = std::get_if<KeyboardEvent>(&it->evt)) {
                    if (keyboardEvt->key == key && keyboardEvt->type == KeyboardEvent::Type::Press) {

                        if (duration > std::chrono::microseconds(0)) {
                            for (auto subIt = it + 1; subIt != m_inputEvents.cend(); ++subIt) {
                                if (auto subKeyboardEvt = std::get_if<KeyboardEvent>(&subIt->evt)) {
                                    if (subKeyboardEvt->key == key && subKeyboardEvt->type == KeyboardEvent::Type::Release && subIt->stamp - it->stamp >= duration) {
                                        return *this;
                                    }
                                }
                            }
                        } else {
                            return *this;
                        }
                    }
                }
            }

            m_containsEvents = false;

            return *this;
        }

        auto KeyRelease(Key key) -> InputBuffer & {
            for (auto it = m_inputEvents.cbegin(); it != m_inputEvents.cend(); ++it) {
                if (auto keyboardEvt = std::get_if<KeyboardEvent>(&it->evt)) {
                    if (keyboardEvt->key == key && keyboardEvt->type == KeyboardEvent::Type::Release) {

                        return *this;

                    }
                }
            }

            m_containsEvents = false;

            return *this;
        }

        auto Consume(std::chrono::microseconds duration) -> bool {
            while (m_inputEvents.front().stamp - m_time >= duration) {
                m_inputEvents.pop_front();
            }

            return m_containsEvents;
        }

        auto Tick(std::chrono::microseconds dt) {
            m_time += dt;
        }

    private:
        std::deque<InputEvent> m_inputEvents;

        bool m_containsEvents{ true };

        std::chrono::system_clock::time_point m_time{ std::chrono::system_clock::now() };
    };
} // namespace Lightning3D

#endif