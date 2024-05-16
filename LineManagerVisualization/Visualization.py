import math
import matplotlib.pyplot as plt
import networkx as nx
#from ParameterSave import ParameterSave

from TopologyRenderer import TopologyRenderer
from CurveRenderer import CurveRenderer


class GraphVisualizer:
    figureNumber = 0
    __parameterString=""

    def __init__(self):
        print("Visualizer init")
        return

    def newFigure(self):
        self.__fig = plt.figure(GraphVisualizer.figureNumber)
        GraphVisualizer.figureNumber += 1
        plt.get_current_fig_manager().window.state('zoomed')

    def show(self):
        plt.show()

    def save(self,path):
        plt.savefig(path+".png",bbox_inches='tight')

    def close(self):
        plt.close(self.__fig)

    def renderStatic(self,nodesStatic, edgesStatic, nodePropertyIndex, edgePropertyIndex):
        nodesStatic = nodesStatic.split("\n")
        edgesStatic = edgesStatic.split("\n")
        self.newFigure()
        TopologyRenderer.renderStatic(nodesStatic, edgesStatic, nodePropertyIndex, edgePropertyIndex)

    def renderDynamic(self, nodesStatic, nodesDynamic, edgesStatic, edgesDynamic, nodePropertyIndex,
                          edgePropertyIndex, times, layout=''):
        nodesStatic = nodesStatic.split("\n")
        nodesDynamic = nodesDynamic.split("\n")
        edgesStatic = edgesStatic.split("\n")
        edgesDynamic = edgesDynamic.split("\n")
        times = times.split(",")
        if layout != '':
            layout = layout.split(",")
            layout[0]=int(layout[0])
            layout[1]=int(layout[1])

        self.newFigure()
        plt.subplots_adjust(left=0.01, bottom=0.01, top=0.97, right=0.99)
        if layout == '':
            # columns=math.ceil(len(times)/(16/9+1))
            # rows=math.ceil(len(times)/columns)
            rows = math.ceil(len(times) / (16 / 9 + 1))
            columns = math.ceil(len(times) / rows)
        else:
            rows = layout[0]
            columns = layout[1]
        for i in range(len(times)):
            sp = self.__fig.add_subplot(rows, columns, i + 1)
            sp.title.set_text("t=" + str(times[i]))
            TopologyRenderer.renderDynamicPlot(nodesStatic, edgesStatic,
                                     nodesDynamic[len(nodesStatic) * i: len(nodesStatic) * (i + 1)],
                                     edgesDynamic[len(edgesStatic) * i: len(edgesStatic) * (i + 1)], nodePropertyIndex,
                                     edgePropertyIndex)


    def renderCurves(self,keys,values,scatter="False",hold="False",logscale="False",legend="",title="",xlabel="",ylabel=""):
        self.newFigure()
        values=values.split("\n")
        keys=keys.split("\n")
        CurveRenderer.renderCurves(keys, values, scatter, hold, logscale, legend, title,xlabel, ylabel)
