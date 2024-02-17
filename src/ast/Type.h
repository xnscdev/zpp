#ifndef AST_TYPE_H
#define AST_TYPE_H

#include "ASTBuilder.h"

#include <llvm/IR/Type.h>

namespace zpp::ast {
class Type {
public:
  Type() = default;
  Type(const Type &) = delete;
  virtual ~Type() = default;
  Type &operator=(const Type &) = delete;
  [[nodiscard]] virtual llvm::Type *resolve(ASTBuilder &a) const = 0;
};

class VoidType final : public Type {
public:
  [[nodiscard]] llvm::Type *resolve(ASTBuilder &a) const override;
};

class IntegerType final : public Type {
public:
  explicit IntegerType(const unsigned int size) : m_size(size) {}
  [[nodiscard]] unsigned int size() const { return m_size; }
  [[nodiscard]] llvm::Type *resolve(ASTBuilder &a) const override;

private:
  unsigned int m_size;
};

class PointerType final : public Type {
public:
  explicit PointerType(std::unique_ptr<Type> type) : m_type(std::move(type)) {}
  [[nodiscard]] const Type &type() const { return *m_type; }
  [[nodiscard]] llvm::Type *resolve(ASTBuilder &a) const override;

private:
  std::unique_ptr<Type> m_type;
};
}

#endif