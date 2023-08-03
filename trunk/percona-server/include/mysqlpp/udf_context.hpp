/* Copyright (c) 2020 Percona LLC and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#ifndef MYSQLPP_UDF_CONTEXT_HPP
#define MYSQLPP_UDF_CONTEXT_HPP

#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

#include <decimal.h>

#include <mysqlpp/common_types.hpp>
#include <mysqlpp/udf_traits.hpp>

namespace mysqlpp {

class udf_context {
  template <typename ImplType, item_result_type ItemResult>
  friend class generic_udf_base;

  template <typename ImplType, item_result_type ItemResult>
  friend class generic_udf;

 public:
  std::size_t get_number_of_args() const noexcept {
    return static_cast<std::size_t>(args_->arg_count);
  }
  item_result_type get_arg_type(std::size_t index) const noexcept {
    return args_->arg_type[index];
  }

  template <item_result_type ItemResult>
  auto get_arg(std::size_t index) const noexcept {
    // TODO: this function can be converted to 'if constexpr' chain instead of
    // tag dispatch when MySQL source code switches to c++17
    assert(get_arg_type(index) == ItemResult);
    return get_arg_impl(index, item_result_tag<ItemResult>{});
  }

  ext::string_view get_attribute(std::size_t index) const noexcept {
    return {args_->attributes[index], args_->attribute_lengths[index]};
  }

  bool is_arg_nullable(std::size_t index) const noexcept {
    return args_->maybe_null[index] != 0;
  }

  bool is_result_nullabale() const noexcept { return initid_->maybe_null; }

  bool is_result_const() const noexcept { return initid_->const_item; }

  std::size_t get_result_max_length() const noexcept {
    return initid_->max_length;
  }

  std::size_t get_result_decimals() const noexcept { return initid_->decimals; }

  bool is_result_decimals_not_fixed() const noexcept {
    return initid_->decimals == DECIMAL_NOT_SPECIFIED;
  }

  void set_arg_type(std::size_t index, item_result_type type) noexcept {
    args_->arg_type[index] = type;
  }

  void mark_arg_nullable(std::size_t index, bool nullable) noexcept {
    args_->maybe_null[index] = (nullable ? 1 : 0);
  }

  void mark_result_nullable(bool nullable) noexcept {
    initid_->maybe_null = nullable;
  }

  void mark_result_const(bool constant) noexcept {
    initid_->const_item = constant;
  }

  void set_result_max_length(std::size_t max_length) noexcept {
    initid_->max_length = max_length;
  }

  void set_result_decimals(std::size_t decimals) noexcept {
    initid_->decimals = decimals;
  }

  void set_result_decimals_not_fixed() noexcept {
    initid_->decimals = DECIMAL_NOT_SPECIFIED;
  }

 private:
  UDF_INIT *initid_;
  UDF_ARGS *args_;

  template <item_result_type ItemResult>
  using item_result_tag = std::integral_constant<item_result_type, ItemResult>;

  using string_result_tag = item_result_tag<STRING_RESULT>;
  using real_result_tag = item_result_tag<REAL_RESULT>;
  using int_result_tag = item_result_tag<INT_RESULT>;
  using decimal_result_tag = item_result_tag<DECIMAL_RESULT>;

  udf_context(UDF_INIT *initid, UDF_ARGS *args) noexcept
      : initid_{initid}, args_{args} {}

  auto get_arg_impl(std::size_t index, string_result_tag) const noexcept {
    return udf_arg_t<STRING_RESULT>{args_->args[index], args_->lengths[index]};
  }
  auto get_arg_impl(std::size_t index, real_result_tag) const noexcept {
    using result_type = udf_arg_t<REAL_RESULT>;
    if (args_->args[index] == nullptr) return result_type{};
    result_type::value_type res;
    std::memcpy(&res, args_->args[index], sizeof res);
    return result_type{res};
  }
  auto get_arg_impl(std::size_t index, int_result_tag) const noexcept {
    using result_type = udf_arg_t<INT_RESULT>;
    if (args_->args[index] == nullptr) return result_type{};
    result_type::value_type res;
    std::memcpy(&res, args_->args[index], sizeof res);
    return result_type{res};
  }
  auto get_arg_impl(std::size_t index, decimal_result_tag) const noexcept {
    return udf_arg_t<DECIMAL_RESULT>{args_->args[index], args_->lengths[index]};
  }
};

}  // namespace mysqlpp

#endif
