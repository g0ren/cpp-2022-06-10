#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Programmer {
protected:
	string profession;
	static const int baseSalary = 100000;
	int calculateSalary() {
		return Programmer::baseSalary * coeff;
	}
	int experience;
	string language;
	float coeff;
	int salary;
public:
	Programmer(string _language, int _experience) :
			language(_language), experience(_experience) {
	}
	void addCoeff(float newCoeff) {
		this->coeff *= newCoeff;
	}
	void updateSalary() {
		this->salary = calculateSalary();
	}
	friend class CategoryDecoratedProgrammer;
};

class FrontEndProgrammer: public Programmer {
public:
	FrontEndProgrammer(string _language, int _experience) :
			Programmer(_language, _experience) {
		this->profession = "Frontend programmer";
		this->coeff = 1.1;
		this->coeff += this->experience * 0.1;
		this->salary = calculateSalary();
	}
	friend class CategoryDecoratedProgrammer;
};

class BackEndProgrammer: public Programmer {

public:
	BackEndProgrammer(string _language, int _experience) :
			Programmer(_language, _experience) {
		this->profession = "Backend programmer";
		this->coeff = 1.2;
		this->coeff += this->experience * 0.1;
		this->salary = calculateSalary();
	}
	friend class CategoryDecoratedProgrammer;
};

class FullStackProgrammer: public Programmer {
public:
	FullStackProgrammer(string _language, int _experience) :
			Programmer(_language, _experience) {
		this->profession = "Full stack programmer";
		this->coeff = 1.3;
		this->coeff += this->experience * 0.1;
		this->salary = calculateSalary();
	}
	friend class CategoryDecoratedProgrammer;
};

class CategoryDecoratedProgrammer {
protected:
	Programmer *admin;
public:
	CategoryDecoratedProgrammer(Programmer *_admin) :
			admin(_admin) {
	}
	string getProfession() {
		return this->admin->profession;
	}
	string getLanguage() {
		return this->admin->language;
	}
	int getSalary() {
		return this->admin->salary;
	}
	int getExperience() {
		return this->admin->experience;
	}
	virtual void show() = 0;
	virtual ~CategoryDecoratedProgrammer() {
	}
};

class JuniorCategoryProgrammer: public CategoryDecoratedProgrammer {
public:
	JuniorCategoryProgrammer(Programmer *_admin) :
			CategoryDecoratedProgrammer(_admin) {
		this->admin->addCoeff(0.9);
		this->admin->updateSalary();
	}
	void show() override {
		cout << "Junior " << this->getProfession() << " on "
				<< this->getLanguage() << ". Job experience "
				<< this->getExperience() << " years. Salary "
				<< this->getSalary() << " rub." << endl;
	}
};

class MiddleCategoryProgrammer: public CategoryDecoratedProgrammer {
public:
	MiddleCategoryProgrammer(Programmer *_admin) :
			CategoryDecoratedProgrammer(_admin) {
		this->admin->addCoeff(1.4);
		this->admin->updateSalary();
	}
	void show() override {
		cout << "Middle " << this->getProfession() << " on "
				<< this->getLanguage() << ". Job experience "
				<< this->getExperience() << " years. Salary "
				<< this->getSalary() << " rub." << endl;
	}
};

class SeniorCategoryProgrammer: public CategoryDecoratedProgrammer {
public:
	SeniorCategoryProgrammer(Programmer *_admin) :
			CategoryDecoratedProgrammer(_admin) {
		this->admin->addCoeff(1.9);
		this->admin->updateSalary();
	}
	void show() override {
		cout << "Senior " << this->getProfession() << " on "
				<< this->getLanguage() << ". Job experience "
				<< this->getExperience() << " years. Salary "
				<< this->getSalary() << " rub." << endl;
	}
};

void generate(vector<CategoryDecoratedProgrammer*> &cont) {
	for (size_t i = 0; i < cont.size(); i++) {
		int lang = rand() % 4;
		int prof = rand() % 3;
		int exp = rand() % 16;
		int chancem { }, chances { };
		if (exp < 5) {
			chancem = 5;
			chances = 10;
		} else if (exp >= 5 && exp < 10) {
			chancem = 2;
			chances = 5;
		} else if (exp >= 10 && exp < 14) {
			chancem = 1;
			chances = 2;
		} else {
			chancem = 0;
			chances = 1;
		}
		Programmer *prog;
		string language;
		switch (lang) {
		case 0:
			language = "C++";
			break;
		case 1:
			language = "Java";
			break;
		case 2:
			language = "Python";
			break;
		case 3:
			language = "PHP";
			break;
		}
		switch (prof) {
		case 0:
			prog = new FrontEndProgrammer(language, exp);
			break;
		case 1:
			prog = new BackEndProgrammer(language, exp);
			break;
		case 2:
			prog = new FullStackProgrammer(language, exp);
			break;
		}
		if (rand() % (chances + 1) == chances)
			cont[i] = new SeniorCategoryProgrammer(prog);
		else if (rand() % (chancem + 1) == chancem)
			cont[i] = new MiddleCategoryProgrammer(prog);
		else
			cont[i] = new JuniorCategoryProgrammer(prog);
	}
}

auto functionalAnd() {
	return []() {
		return true;
	};
}

template<typename Function>
auto functionalAnd(Function function) {
	return [function](auto const &value) {
		return function(value);
	};
}

template<typename Function1, typename Function2>
auto functionalAnd(Function1 function1, Function2 function2) {
	return [function1, function2](auto const &value) {
		return function1(value) && function2(value);
	};
}

template<typename Function, typename ... Functions>
auto functionalAnd(Function function, Functions ... functions) {
	return functionalAnd(function, functionalAnd(functions...));
}

template<typename UnaryPredicate, typename ... UnaryPredicates>
vector<CategoryDecoratedProgrammer*>::iterator find_if_many_criteria(
		vector<CategoryDecoratedProgrammer*> cont, UnaryPredicate first,
		UnaryPredicates ... rest) {
	return find_if(cont.begin(), cont.end(), functionalAnd(first, rest...));
}

int main() {
	srand(time(NULL));
	vector<CategoryDecoratedProgrammer*> vect(10);
	generate(vect);
	for (auto p : vect)
		p->show();
	auto lambda1 = [](CategoryDecoratedProgrammer *prog) {
		return prog->getLanguage() == "PHP";
	};
	auto lambda2 = [](CategoryDecoratedProgrammer *prog) {
		return prog->getExperience() <= 12;
	};
	int threshold { 150000 };
	auto lambda3 = [threshold](CategoryDecoratedProgrammer *prog) {
		return prog->getSalary() >= threshold;
	};

	cout << "---___---___---___---" << endl;

	(*find_if_many_criteria(vect, lambda1, lambda2, lambda3))->show();
	return 0;
}
