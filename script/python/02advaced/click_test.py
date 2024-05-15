import click

# 执行程序时，录入参数

@click.command()
@click.option("--count", default = 1, help = "欢迎次数")
@click.option("--name", prompt = "您的名字是", help = "用户的名称")

def hello(count, name) :
    for x in range(count) :
        click.echo("hello %s!" % name)

if __name__ == "__main__" :
    hello()

















