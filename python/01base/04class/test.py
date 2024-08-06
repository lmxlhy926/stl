#! /usr/bin/python3

class wahser():
    def __init__(self, height, width) -> None:
        self.width = 100
        self.height = 200
        self.list = []

    def __del__(self):
        print("析构")

    def __str__(self) -> str:
        return "self.width: {}, self.height: {}, self.list: {}".format(self.width, self.height, self.list)

    def addList(self, item:str) -> None:
        self.list.append(item)

haier = wahser(100, 200)
haier.addList('first')
haier.addList("second")
print(haier)






















