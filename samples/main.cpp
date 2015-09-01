#include <iostream>
#include <memory>

#include "Compiler/Message.h"
#include "Compiler/ProtoParser.h"
#include "Proto/MessageReflection.h"
#include "Proto/MessageFactory.h"
#include "Proto/SerializedMessage.h"
#include "Proto/SerializedPrimitive.h"


#include "tiny_pb.h"
#include "simple_pb.h"

using namespace proto;

void test_Basic() {
  // test singular numeric field;
  AA::BB::DogInfo dog1;
  std::cout << dog1.has_age() << std::endl;
  dog1.set_name("snoopy");
  dog1.set_age(3);
  std::cout << dog1.has_age() << std::endl;
  dog1.clear_age();
  std::cout << dog1.has_age() << std::endl;
  std::cout << std::endl;

  // test singular message field
  std::cout << dog1.has_family() << std::endl;
  std::cout << "dog1.family ptr = " << dog1.mutable_family() << std::endl;
  const AA::FamilyInfo& family = dog1.family();
  std::cout << "default family: (" << family.address() << ", "
            << family.numberdogs() << ") " << std::endl;
  std::cout << dog1.has_family() << std::endl;
  dog1.clear_family();
  std::cout << dog1.has_family() << std::endl;
  
  AA::FamilyInfo* new_family = new AA::FamilyInfo();
  new_family->set_address("Shishan Road");
  new_family->set_numberdogs(1);
  std::cout << "setting allocated family Shishan Road" << std::endl;
  dog1.set_allocated_family(new_family);
  std::cout << dog1.has_family() << std::endl;
  auto fp = dog1.release_family();
  std::cout << "released family: (" << fp->address() << ", "
            << fp->numberdogs() << ") " << std::endl;
  delete fp;
  std::cout << dog1.has_family() << std::endl;
  std::cout << std::endl << std::endl;

  // test repeated string field
  dog1.add_alias("doggy");
  dog1.add_alias("snooqi");
  std::cout << "dog1 alias: ";
  for (auto& alias: dog1.alias()) {
    std::cout << alias << " ";
  }
  std::cout << std::endl << std::endl;

  // test repeated message field
  AA::FamilyInfo* new_neighbor = dog1.add_neighbors();
  new_neighbor->set_address("Shishan Huating");
  new_neighbor->set_numberdogs(1);
  new_neighbor = dog1.add_neighbors();
  new_neighbor->set_address("Fumin Xiaoqu");
  new_neighbor->set_numberdogs(2);
  for (auto& neighbor: dog1.neighbors()) {
    std::cout << "(" << neighbor.address() << ", "
              << neighbor.numberdogs()
              << ") ";
  }
  std::cout << std::endl;

  AA::FamilyInfo* neighbor = dog1.mutable_neighbors(1);
  neighbor->set_address("Chengbei Xincun");
  for (auto& neighbor: dog1.neighbors()) {
    std::cout << "(" << neighbor.address() << ", "
              << neighbor.numberdogs()
              << ") ";
  }
  std::cout << std::endl;
}

#define PRINT_HAS(obj, field, obj_name, field_name)                       \
  if (obj.has_##field()) {                                                \
    std::cout << obj_name << " has " << field_name << std::endl;          \
  }                                                                       \
  else {                                                                  \
    std::cout << obj_name << " not has " << field_name << std::endl;      \
  }

void test_Serialize() {
  std::cout << __FUNCTION__ << "() ..." << std::endl;

  ::HaiZhong::Student stu1;
  stu1.set_name("hy");
  stu1.set_age(24);
  stu1.set_xyz(-3);
  stu1.set_def(17375839784);
  stu1.set_ghi(-73159748968);
  stu1.set_graduated(true);
  stu1.set_weight(70.5);
  stu1.set_sex(::HaiZhong::Student::FEMALE);

  ::proto::SerializedMessage* sdmsg = stu1.Serialize();
  const char* obj_data = sdmsg->GetBytes();
  for (unsigned int i = 0; i < sdmsg->size(); i++) {
    printf("0x%x ", obj_data[i] & 0xff);
  }
  printf("\n");
  ::HaiZhong::Student stu2;
  stu2.DeSerialize(obj_data, sdmsg->size());
  std::cout << "stu2.name() = " << stu2.name() << std::endl;
  std::cout << "stu2.age() = " << stu2.age() << std::endl;
  std::cout << "stu2.xyz() = " << stu2.xyz() << std::endl;
  std::cout << "stu2.def() = " << stu2.def() << std::endl;
  std::cout << "stu2.ghi() = " << stu2.ghi() << std::endl;
  std::cout << "stu2.graduated() = " << stu2.graduated() << std::endl;
  std::cout << "stu2.weight() = " << stu2.weight() << std::endl;
  std::cout << "stu2.sex() = " << stu2.sex() << std::endl;
  PRINT_HAS(stu2, name, "stu2", "name")
  PRINT_HAS(stu2, age, "stu2", "age")
  PRINT_HAS(stu2, xyz, "stu2", "xyz")
  PRINT_HAS(stu2, def, "stu2", "deg")
  PRINT_HAS(stu2, ghi, "stu2", "ghi")
  PRINT_HAS(stu2, graduated, "stu2", "graduated")
  PRINT_HAS(stu2, weight, "stu2", "weight")
  PRINT_HAS(stu2, sex, "stu2", "sex")
  std::cout << "deleting sdmsg ..." << std::endl;
  delete sdmsg;
}

int main() {
  std::cout << "-------------------- Test ----------------------" << std::endl;
  test_Serialize();
  return 0;
}