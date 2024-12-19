#! /usr/bin/python3

from pyecharts import options as opts
from pyecharts.charts import Bar

x_data = ["一月", "二月", "三月", "四月", "五月"]
y_data = [10, 20, 15, 25, 30]

bar_chart = Bar()
bar_chart.add_xaxis(x_data)
bar_chart.add_yaxis("销售额", y_data)

bar_chart.set_global_opts(
    title_opts=opts.TitleOpts(title="月度销售额柱状图"),
    xaxis_opts=opts.AxisOpts(name="月份"),
    yaxis_opts=opts.AxisOpts(name="销售额(万元)"),
    legend_opts=opts.LegendOpts(pos_left="center", pos_top="top"),
    toolbox_opts=opts.ToolboxOpts,
    tooltip_opts=opts.TooltipOpts(trigger="axis", axis_pointer_type="cross")
)


bar_chart.render()


























