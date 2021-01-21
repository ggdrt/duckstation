#pragma once
#include "common/types.h"
#include "common/state_wrapper.h"
#include "controller.h"
#include "memory_card.h"
#include <array>

class Multitap final
{
public:
  Multitap(u32 index);

  void Reset();

  ALWAYS_INLINE void SetEnable(bool enable) { m_enabled = enable; };
  ALWAYS_INLINE bool IsEnabled() const { return m_enabled; };

  bool DoState(StateWrapper& sw);

  void ResetTransferState();
  bool Transfer(const u8 data_in, u8* data_out);
  ALWAYS_INLINE bool IsReadingMemoryCard() { return m_enabled && m_transfer_state == TransferState::MemoryCard; };

private:
  ALWAYS_INLINE static constexpr u8 GetMultitapIDByte() { return 0x80; };
  ALWAYS_INLINE static constexpr u8 GetStatusByte() { return 0x5A; };

  Controller* GetControllerForSlot(u32 slot) const;
  MemoryCard* GetMemoryCardForSlot(u32 slot) const;

  bool TransferController(u32 slot, const u8 data_in, u8* data_out) const;
  bool TransferMemoryCard(u32 slot, const u8 data_in, u8* data_out) const;

  enum class TransferState : u8
  {
    Idle,
    MemoryCard,
    ControllerCommand,
    SingleController,
    AllControllers
  };

  TransferState m_transfer_state = TransferState::Idle;
  u8 m_selected_slot = 0;

  u32 m_controller_transfer_step = 0;

  bool m_invalid_transfer_all_command = false;
  bool m_transfer_all_controllers = false;
  bool m_current_controller_done = false;

  std::array<u8, 32> m_transfer_buffer{};

  u32 m_index;
  bool m_enabled = false;
};
