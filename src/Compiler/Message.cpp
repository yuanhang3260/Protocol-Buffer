#include <iostream>
#include <algorithm>
#include <vector>

#include "../Utility/Strings.h"
#include "Message.h"

namespace PandaProto {
namespace Compiler {

Message::Message(std::string name, std::string package) :
    PbType(name, package) {
}

Message::~Message() {}

bool Message::AddField(std::shared_ptr<MessageField> field) {
  if (fields_map_.find(field->name()) != fields_map_.end()) {
    fprintf(stderr,
            "ERROR: field name \"%s\" already exisits in Message \"%s\"\n", 
            field->name().c_str(), name_.c_str());
    return false;
  }
  fields_map_[field->name()] = field;
  fileds_list_.push_back(field);
  return true;
}

bool Message::AddEnum(std::shared_ptr<EnumType> newenum) {
  if (enums_map_.find(newenum->name()) != enums_map_.end()) {
    fprintf(stderr,
            "ERROR: enum name \"%s\" already exisits in Message \"%s\"\n", 
            newenum->name().c_str(), name_.c_str());
    return false;
  }
  enums_map_[newenum->name()] = newenum;
  return true;
}

bool FieldComparatorByTag(const std::shared_ptr<MessageField>& field1,
                          const std::shared_ptr<MessageField>& field2) {
  return field1->tag() < field2->tag();
}

void Message::Print() {
  std::cout << "Message " << name_ << " in Package " << package_ << std::endl;
  std::sort(fileds_list_.begin(), fileds_list_.end(), FieldComparatorByTag);
  // Print enums defined in this message.
  for (auto& e: enums_map_) {
    e.second->Print();
  }
  // Print message fields.
  for (auto& field: fileds_list_) {
    std::cout << "  " << MessageField::GetModifierAsString(field->modifier());
    std::cout << " " << field->type_name();
    std::cout << " " << field->name() << " = " << field->tag();
    if (field->default_value() != "") {
      std::cout << " [ default = " << field->default_value() << " ]";
    }
    std::cout << std::endl;
  }
}

MessageField* Message::FindMessage(std::string name) const {
  if (fields_map_.find(name) == fields_map_.end()) {
    return NULL;
  }
  return fields_map_.at(name).get();
}

EnumType* Message::FindEnumType(std::string name) const {
  if (enums_map_.find(name) == enums_map_.end()) {
    return NULL;
  }
  return enums_map_.at(name).get();
}

const std::vector<std::shared_ptr<MessageField>>&
Message::fields_list() const {
  return fileds_list_;
}

const std::map<std::string, std::shared_ptr<EnumType>>&
Message::enums_map() const {
  return enums_map_;
}


}  // namespace Compiler
}  // namespace PandaProto