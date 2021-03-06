// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TEST_CCTEST_INTERPRETER_BYTECODE_EXPECTATIONS_PRINTER_H_
#define TEST_CCTEST_INTERPRETER_BYTECODE_EXPECTATIONS_PRINTER_H_

#include <iostream>
#include <string>
#include <vector>

#include "src/interpreter/bytecodes.h"
#include "src/objects.h"

namespace v8 {

class Isolate;

namespace internal {
namespace interpreter {

class BytecodeArrayIterator;

class BytecodeExpectationsPrinter final {
 public:
  enum class ConstantPoolType {
    kUnknown,
    kString,
    kNumber,
    kMixed,
  };

  BytecodeExpectationsPrinter(v8::Isolate* i,
                              ConstantPoolType t = ConstantPoolType::kMixed)
      : isolate_(i),
        const_pool_type_(t),
        execute_(true),
        wrap_(true),
        test_function_name_(kDefaultTopFunctionName) {}

  void PrintExpectation(std::ostream& stream,  // NOLINT
                        const std::string& snippet) const;

  void set_constant_pool_type(ConstantPoolType const_pool_type) {
    const_pool_type_ = const_pool_type;
  }
  ConstantPoolType const_pool_type() const { return const_pool_type_; }

  void set_execute(bool execute) { execute_ = execute; }
  bool execute() const { return execute_; }

  void set_wrap(bool wrap) { wrap_ = wrap; }
  bool wrap() const { return wrap_; }

  void set_top_level(bool top_level) { top_level_ = top_level; }
  bool top_level() const { return top_level_; }

  void set_test_function_name(const std::string& test_function_name) {
    test_function_name_ = test_function_name;
  }
  std::string test_function_name() const { return test_function_name_; }

 private:
  void PrintEscapedString(std::ostream& stream,  // NOLINT
                          const std::string& string) const;
  void PrintBytecodeOperand(std::ostream& stream,  // NOLINT
                            const BytecodeArrayIterator& bytecode_iter,
                            const Bytecode& bytecode, int op_index,
                            int parameter_count) const;
  void PrintBytecode(std::ostream& stream,  // NOLINT
                     const BytecodeArrayIterator& bytecode_iter,
                     int parameter_count) const;
  void PrintV8String(std::ostream& stream,  // NOLINT
                     i::String* string) const;
  void PrintConstant(std::ostream& stream,  // NOLINT
                     i::Handle<i::Object> constant) const;
  void PrintFrameSize(std::ostream& stream,  // NOLINT
                      i::Handle<i::BytecodeArray> bytecode_array) const;
  void PrintBytecodeSequence(std::ostream& stream,  // NOLINT
                             i::Handle<i::BytecodeArray> bytecode_array) const;
  void PrintConstantPool(std::ostream& stream,  // NOLINT
                         i::FixedArray* constant_pool) const;
  void PrintCodeSnippet(std::ostream& stream,  // NOLINT
                        const std::string& body) const;
  void PrintBytecodeArray(std::ostream& stream,  // NOLINT
                          i::Handle<i::BytecodeArray> bytecode_array) const;
  void PrintHandlers(std::ostream& stream,  // NOLINT
                     i::Handle<i::BytecodeArray> bytecode_array) const;

  v8::Local<v8::String> V8StringFromUTF8(const char* data) const;
  std::string WrapCodeInFunction(const char* function_name,
                                 const std::string& function_body) const;

  v8::Local<v8::Script> Compile(const char* program) const;
  void Run(v8::Local<v8::Script> script) const;
  i::Handle<i::BytecodeArray> GetBytecodeArrayForGlobal(
      const char* global_name) const;
  i::Handle<v8::internal::BytecodeArray> GetBytecodeArrayForScript(
      v8::Local<v8::Script> script) const;

  i::Isolate* i_isolate() const {
    return reinterpret_cast<i::Isolate*>(isolate_);
  }

  v8::Isolate* isolate_;
  ConstantPoolType const_pool_type_;
  bool execute_;
  bool wrap_;
  bool top_level_;
  std::string test_function_name_;

  static const char* const kDefaultTopFunctionName;
};

}  // namespace interpreter
}  // namespace internal
}  // namespace v8

#endif  // TEST_CCTEST_INTERPRETER_BYTECODE_EXPECTATIONS_PRINTER_H_
