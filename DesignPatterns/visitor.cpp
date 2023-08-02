#include <iostream>
#include <vector>

class Computer;
class Monitor;
class Keyboard;
class Mouse;

class ComputerPartVisitor {
public:
	virtual void visit(Computer& comp) = 0;
	virtual void visit(Monitor& monitor) = 0;
	virtual void visit(Keyboard& keyb) = 0;
	virtual void visit(Mouse& mouse) = 0;
};

class ComputerPart {
public:
	virtual void accept(ComputerPartVisitor* cpv) = 0;
};

class Keyboard: public ComputerPart {
public:
	void accept(ComputerPartVisitor* cpv) override {
		cpv->visit(*this);
	}
};

class Monitor: public ComputerPart {
public:
	void accept(ComputerPartVisitor* cpv) override {
		cpv->visit(*this);
	}
};

class Mouse: public ComputerPart {
public:
	void accept(ComputerPartVisitor* cpv) override {
		cpv->visit(*this);
	}
};

class Computer: public ComputerPart {
public:
	std::vector<ComputerPart*> parts;
	Computer() {
		parts = {new Keyboard(), new Mouse(), new Monitor()};
	}

	void accept(ComputerPartVisitor* cpv) override {
		for (auto p: parts) {
			p->accept(cpv);
		}
	}
};



class ComputerPartDisplayVisitor: public ComputerPartVisitor {
	void visit(Computer& comp) override {
		std::cout << "Computer.\n";
	}
	void visit(Monitor& monitor) override {
		std::cout << "Monitor.\n";
	}
	void visit(Keyboard& keyb) override {
		std::cout << "Keyboard.\n";
	}
	void visit(Mouse& mouse) override {
		std::cout << "Mouse.\n";
	}
};

int main() {
	ComputerPart* cm = new Computer();
	cm->accept(new ComputerPartDisplayVisitor());
	return 0;
}
