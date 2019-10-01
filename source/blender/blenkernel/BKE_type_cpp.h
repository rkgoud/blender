#ifndef __BKE_DATA_TYPE_H__
#define __BKE_DATA_TYPE_H__

#include "BLI_string_ref.h"
#include "BLI_utility_mixins.h"
#include "BLI_vector.h"

namespace BKE {

using BLI::StringRef;
using BLI::StringRefNull;

class TypeCPP final {
 public:
  using ConstructDefaultF = void (*)(void *ptr);
  using DestructF = void (*)(void *ptr);
  using CopyToInitializedF = void (*)(void *src, void *dst);
  using CopyToUninitializedF = void (*)(void *src, void *dst);
  using RelocateToInitializedF = void (*)(void *src, void *dst);
  using RelocateToUninitializedF = void (*)(void *src, void *dst);

  TypeCPP(std::string name,
          uint size,
          uint alignment,
          bool trivially_destructible,
          ConstructDefaultF construct_default,
          DestructF destruct,
          CopyToInitializedF copy_to_initialized,
          CopyToUninitializedF copy_to_uninitialized,
          RelocateToInitializedF relocate_to_initialized,
          RelocateToUninitializedF relocate_to_uninitialized)
      : m_size(size),
        m_alignment(alignment),
        m_trivially_destructible(trivially_destructible),
        m_construct_default(construct_default),
        m_destruct(destruct),
        m_copy_to_initialized(copy_to_initialized),
        m_copy_to_uninitialized(copy_to_uninitialized),
        m_relocate_to_initialized(relocate_to_initialized),
        m_relocate_to_uninitialized(relocate_to_uninitialized),
        m_name(name)
  {
    BLI_assert(is_power_of_2_i(m_alignment));

    m_alignment_mask = m_alignment - 1;
  }

  StringRefNull name() const
  {
    return m_name;
  }

  uint size() const
  {
    return m_size;
  }

  uint alignment() const
  {
    return m_alignment;
  }

  bool trivially_destructible() const
  {
    return m_trivially_destructible;
  }

  bool pointer_has_valid_alignment(void *ptr) const
  {
    return (POINTER_AS_UINT(ptr) & m_alignment_mask) == 0;
  }

  void construct_default(void *ptr) const
  {
    BLI_assert(this->pointer_has_valid_alignment(ptr));

    m_construct_default(ptr);
  }

  void destruct(void *ptr) const
  {
    BLI_assert(this->pointer_has_valid_alignment(ptr));

    m_destruct(ptr);
  }

  void copy_to_initialized(void *src, void *dst) const
  {
    BLI_assert(this->pointer_has_valid_alignment(src));
    BLI_assert(this->pointer_has_valid_alignment(dst));

    m_copy_to_initialized(src, dst);
  }

  void copy_to_uninitialized(void *src, void *dst) const
  {
    BLI_assert(this->pointer_has_valid_alignment(src));
    BLI_assert(this->pointer_has_valid_alignment(dst));

    m_copy_to_uninitialized(src, dst);
  }

  void relocate_to_initialized(void *src, void *dst) const
  {
    BLI_assert(this->pointer_has_valid_alignment(src));
    BLI_assert(this->pointer_has_valid_alignment(dst));

    m_relocate_to_initialized(src, dst);
  }

  void relocate_to_uninitialized(void *src, void *dst) const
  {
    BLI_assert(this->pointer_has_valid_alignment(src));
    BLI_assert(this->pointer_has_valid_alignment(dst));

    m_relocate_to_uninitialized(src, dst);
  }

 private:
  uint m_size;
  uint m_alignment;
  uint m_alignment_mask;
  bool m_trivially_destructible;
  ConstructDefaultF m_construct_default;
  DestructF m_destruct;
  CopyToInitializedF m_copy_to_initialized;
  CopyToUninitializedF m_copy_to_uninitialized;
  RelocateToInitializedF m_relocate_to_initialized;
  RelocateToUninitializedF m_relocate_to_uninitialized;
  std::string m_name;
};

}  // namespace BKE

#endif /* __BKE_DATA_TYPE_H__ */