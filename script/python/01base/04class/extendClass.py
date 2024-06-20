

class person:
    name = None
    age = None
    weight = None

    def __init__(self, name, age, weight) -> None:
        print("---person 构造---")
        self.name = name
        self.age = age
        self.weight = weight
        print("person: id(name): {}".format(id(self.name)))
    
    def __del__(self):
        print("---person 析构---")

    def show(self):
        print("--person: name:{}, age:{}, weight:{}".format(self.name, self.age, self.weight))
    


class student(person):
    name = None
    grade = None

    def __init__(self, name, age, weight, grade) -> None:
        print("--student 构造")
        super().__init__(name, age, weight)
        # person.__init__(self, name, age, weight)
        self.name = name + "-student"
        self.grade = grade
        print("person: id(name):{}; student: id(name):{}".format(id(super().name), id(self.name)))

    def __del__(self):
        super().__del__()
        # person.__del__(self)
        print("--student 析构")
        

    def show(self):
        print("--student: name:{}, grade:{}".format(self.name, self.grade))























