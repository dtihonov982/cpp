#include <iostream>
#include <memory>

class Student {
public:
    std::string getRollNo() {
        return rollNo_;
    }
    void setRollNo(const std::string& rollNo) {
        rollNo_ = rollNo;
    }
    std::string getName() {
        return name_;
    }
    void setName(const std::string& name) {
        name_ = name;
    }
private:
    std::string rollNo_;
    std::string name_;
};

class StudentView {
public:
    void printStudentDetails(const std::string& studentName, const std::string& studentRollNo) {
        std::cout << "Student ";
        std::cout << " Name: " << studentName << " ";
        std::cout << "Roll No:" << studentRollNo << std::endl;
    }
};

class StudentController {
public:
    StudentController(const std::shared_ptr<Student>& student, const std::shared_ptr<StudentView>& view)
    : model_(student), view_(view) {}

    void setStudentName(const std::string& name) {
        model_->setName(name);
    }

    std::string getStudentName() {
        return model_->getName();
    }

    void setStudentRollNo(const std::string& rollNo) {
        model_->setRollNo(rollNo);
    }

    std::string getStudentRollNo() {
        return model_->getRollNo();
    }

    void updateView() {
        view_->printStudentDetails(model_->getName(), model_->getRollNo());
    }
private:
    std::shared_ptr<Student> model_;
    std::shared_ptr<StudentView> view_;
};

std::shared_ptr<Student> retriveStudentFromDb() {
    std::shared_ptr<Student> student = std::make_shared<Student>();
    student->setName("Robert");
    student->setRollNo("10");
    return student;
}

int main() {
    auto student = retriveStudentFromDb();
    std::shared_ptr<StudentView> view = std::make_shared<StudentView>();
    StudentController controller(student, view);

    controller.updateView();
    controller.setStudentName("John");
    controller.updateView();

    return 0;

}
