#include <iostream>
#include <memory>
#include <utility>
#include <string_view>
#include <vector>
#include <algorithm>
#include <unordered_set>

class Person {
private:
    std::string name_;
    std::string gender_;
    std::string maritalStatus_;
public:
    Person(std::string_view name, std::string_view gender, std::string_view maritalStatus)
    : name_(name), gender_(gender), maritalStatus_(maritalStatus) {
    }
    std::string_view getName() const { return name_; }
    std::string_view getGender() const { return gender_; }
    std::string_view getMaritalStatus() const { return maritalStatus_; }
};

bool operator==(const Person& lhs, const Person& rhs) {
    return lhs.getName() == rhs.getName();
}

class Criteria {
public:
    virtual std::vector<Person> meetCriteria(const std::vector<Person>& persons) = 0;
    virtual ~Criteria() {}
};

class CriteriaMale: public Criteria {
public:
    std::vector<Person> meetCriteria(const std::vector<Person>& persons) override {
        std::vector<Person> malePerson;
        std::copy_if(persons.begin(),
                    persons.end(),
                    std::back_inserter(malePerson),
                    [] (const Person& p) { return p.getGender() == "male"; } );
        return malePerson;
    }
};
        
        
class CriteriaFemale: public Criteria {
public:
    std::vector<Person> meetCriteria(const std::vector<Person>& persons) override {
        std::vector<Person> femalePerson;
        std::copy_if(persons.begin(),
                    persons.end(),
                    std::back_inserter(femalePerson),
                    [] (const Person& p) { return p.getGender() == "female"; } );
        return femalePerson;
    }
};
        

class CriteriaSingle: public Criteria {
public:
    std::vector<Person> meetCriteria(const std::vector<Person>& persons) override {
        std::vector<Person> singlePerson;
        std::copy_if(persons.begin(),
                    persons.end(),
                    std::back_inserter(singlePerson),
                    [] (const Person& p) { return p.getMaritalStatus() == "single"; } );
        return singlePerson;
    }
};
        
class AndCriteria: public Criteria {
public:
    AndCriteria(std::shared_ptr<Criteria>& first, std::shared_ptr<Criteria>& second)
    : first_(first), second_(second) {}

    std::vector<Person> meetCriteria(const std::vector<Person>& persons) override {
        std::vector<Person> firstCriteriaPersons = first_->meetCriteria(persons);
        return second_->meetCriteria(firstCriteriaPersons);
    }
private:
    std::shared_ptr<Criteria> first_;
    std::shared_ptr<Criteria> second_;
};
        
class OrCriteria: public Criteria {
public:
    OrCriteria(std::shared_ptr<Criteria>& first, std::shared_ptr<Criteria>& second)
    : first_(first), second_(second) {}

    std::vector<Person> meetCriteria(const std::vector<Person>& persons) override {
        std::vector<Person> firstCriteriaPersons = first_->meetCriteria(persons);
        std::vector<Person> secondCriteriaPersons = second_->meetCriteria(persons);
        for (auto& p: secondCriteriaPersons) {
            auto it = std::find(firstCriteriaPersons.begin(),
                                firstCriteriaPersons.end(),
                                p);
            if (it == firstCriteriaPersons.end())
                firstCriteriaPersons.push_back(p);
        }
        return firstCriteriaPersons;
    }
private:
    std::shared_ptr<Criteria> first_;
    std::shared_ptr<Criteria> second_;
};

void printPersons(const std::vector<Person>& persons) {
    for (auto& p: persons) {
        std::cout << p.getName() << '\n';
    }
}
        

int main() {
    std::vector<Person> persons;
    persons.push_back(Person("Robert","male", "single"));
    persons.push_back(Person("John", "male", "married"));
    persons.push_back(Person("Laura", "female", "married"));
    persons.push_back(Person("Diana", "female", "single"));
    persons.push_back(Person("Mike", "male", "single"));
    persons.push_back(Person("Bobby", "male", "single"));

    std::shared_ptr<Criteria> male(new CriteriaMale{});
    std::shared_ptr<Criteria> female(new CriteriaFemale{});
    std::shared_ptr<Criteria> single(new CriteriaSingle{});
    #if 1
    std::shared_ptr<Criteria> singleMale(new AndCriteria{single, male});
    std::shared_ptr<Criteria> singleOrFemale(new OrCriteria{single, female});
    #endif
    
    printPersons(male->meetCriteria(persons));
    std::cout << '\n';
    printPersons(female->meetCriteria(persons));
    std::cout << '\n';
    #if 1
    printPersons(singleMale->meetCriteria(persons));
    std::cout << '\n';
    printPersons(singleOrFemale->meetCriteria(persons));
    #endif

    return 0;
}
