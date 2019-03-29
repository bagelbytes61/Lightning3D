//#include "LEKeyboard.h"
//
//#include "LEKeyboard.h"
//
//using namespace Lightning3D::Engine;
//
//CKeyboard::CKeyboard(CInputBuffer* _pibBuffer) 
//    : m_pibBuffer(_pibBuffer) {
//
//}
//
//CKeyboard::~CKeyboard() {
//
//}
//
//void CKeyboard::OnKeyPress(uint32_t _ui32Keycode) {
//    LE_KEY_EVENT keEvent = { };
//    keEvent.ketType = LE_KET_PRESS;
//    keEvent.ui64Time = m_tTime.GetRealMicros();
//
//    std::scoped_lock<std::mutex> slLock(m_mMutex);
//
//    m_vkeEvents[_ui32Keycode].push_back(keEvent);
//}
//
//void CKeyboard::OnKeyRelease(uint32_t _ui32Keycode) {
//    LE_KEY_EVENT keEvent = { };
//    keEvent.ketType = LE_KET_RELEASE;
//    keEvent.ui64Time = m_tTime.GetRealMicros();
//
//    std::scoped_lock<std::mutex> slLock(m_mMutex);
//
//    m_vkeEvents[_ui32Keycode].push_back(keEvent);
//}