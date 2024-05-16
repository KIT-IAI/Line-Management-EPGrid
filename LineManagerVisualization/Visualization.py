import math
import matplotlib.pyplot as plt
import networkx as nx

#from ParameterSave import ParameterSave


class GraphVisualizer:
    figureNumber = 0
    __parameterString=""

    def __init__(self):
        print("Visualizer init")
        return

    def show(self):
        plt.show()

    def save(self,path):
        plt.savefig(path+".png",bbox_inches='tight')

    def close(self):
        plt.close(self.__fig)

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

        self.__fig = plt.figure(GraphVisualizer.figureNumber)
        GraphVisualizer.figureNumber += 1
        plt.get_current_fig_manager().window.state('zoomed')
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
            self.__renderDynamicPlot(nodesStatic, edgesStatic,
                                     nodesDynamic[len(nodesStatic) * i: len(nodesStatic) * (i + 1)],
                                     edgesDynamic[len(edgesStatic) * i: len(edgesStatic) * (i + 1)], nodePropertyIndex,
                                     edgePropertyIndex)

    def __renderDynamicPlot(self, nodesStatic, edgesStatic, nodesDynamic, edgesDynamic, nodePropertyIndex,
                            edgePropertyIndex):
        self.__g = nx.DiGraph()
        couplingIndex = 0
        houseIndex = 0
        n = ""
        for i in range(len(nodesStatic)):
            n = nodesStatic[i].split(",")
            if n[1] == "CC":
                pos = [houseIndex, 0]
                houseIndex += 1
                color = 'pink'
            elif n[1] == "VC":
                pos = [houseIndex, 0]
                houseIndex += 1
                color = '#879dff'
            else:
                pos = [couplingIndex, 1]
                couplingIndex += 1
                color = '#b0ada4'
            self.__g.add_node(n[0], pos=pos, color=color, label=nodesDynamic[i].split(",")[int(nodePropertyIndex)])
        e = ""
        for i in range(len(edgesStatic)):
            e = edgesStatic[i].split(",")
            v1id = e[0]
            v2id = e[1]
            edgeWeight = abs(float(edgesDynamic[i].split(',')[int(edgePropertyIndex)]))
            current = float(edgesDynamic[i].split(',')[1])
            absCurrent = abs(current)
            limit = float(e[3])
            # print("Current: "+str(absCurrent)+" Limit: "+str(limit))
            if absCurrent > limit:
                color = '#ff0000'
            elif absCurrent >= limit * 0.90:
                color = '#ffa600'
            elif absCurrent >= limit * 0.80:
                color = '#e3e300'
            elif absCurrent >= limit * 0.70:
                color = '#139e13'
            elif absCurrent > 0:
                color = '#0000ff'
            else:
                color = 'black'
            # self.__g.add_edge(v1id, v2id, weight=edgeWeight, color=color)
            if current > 0:
                self.__g.add_edge(v1id, v2id, weight=edgeWeight, color=color)
            else:
                self.__g.add_edge(v2id, v1id, weight=edgeWeight, color=color)

        if len(nodesStatic) > 20:  # TODO this should be a separate  parameter and not be detected by node count
            pos = nx.planar_layout(self.__g)
        else:
            pos = nx.get_node_attributes(self.__g, 'pos')
        node_colors = nx.get_node_attributes(self.__g, 'color')
        node_labels = nx.get_node_attributes(self.__g, 'label')
        edge_colors = nx.get_edge_attributes(self.__g, 'color')
        edge_labels = nx.get_edge_attributes(self.__g, 'weight')
        nx.draw_networkx(self.__g, pos, labels=node_labels, node_color=node_colors.values(),
                         edge_color=edge_colors.values(), width=1.5, arrowsize=15)
        nx.draw_networkx_edge_labels(self.__g, pos, edge_labels=edge_labels)

    def renderStatic(self, nodesStatic, edgesStatic, nodePropertyIndex, edgePropertyIndex):
        nodesStatic = nodesStatic.split("\n")
        edgesStatic = edgesStatic.split("\n")
        self.__fig = plt.figure(GraphVisualizer.figureNumber)
        GraphVisualizer.figureNumber += 1
        plt.get_current_fig_manager().window.state('zoomed')

        self.__g = nx.Graph()
        couplingIndex = 0
        houseIndex = 0
        for n in nodesStatic:
            n = n.split(",")
            if n[1] == "CC":
                pos = [houseIndex, 0]
                houseIndex += 1
                color = 'pink'
            elif n[1] == "VC":
                pos = [houseIndex, 0]
                houseIndex += 1
                color = '#879dff'
            else:
                pos = [couplingIndex, 1]
                couplingIndex += 1
                color = '#b0ada4'
            self.__g.add_node(n[0], pos=pos, color=color, label=n[int(nodePropertyIndex)])

        for e in edgesStatic:
            e = e.split(",")
            self.__g.add_edge(e[0], e[1], weight=e[int(edgePropertyIndex)], color='black')

        if len(nodesStatic) > 20:  # TODO this should be a separate  parameter and not be detected by node count
            pos = nx.planar_layout(self.__g)
        else:
            pos = nx.get_node_attributes(self.__g, 'pos')

        node_colors = nx.get_node_attributes(self.__g, 'color')
        node_labels = nx.get_node_attributes(self.__g, 'label')
        edge_colors = nx.get_edge_attributes(self.__g, 'color')
        labels = nx.get_edge_attributes(self.__g, 'weight')
        nx.draw_networkx(self.__g, pos, labels=node_labels, node_color=node_colors.values(),
                         edge_color=edge_colors.values(), width=1.5)
        nx.draw_networkx_edge_labels(self.__g, pos, edge_labels=labels)

    def renderCurves(self,keys,values,scatter="False",hold="False",logscale="False",legend="",title="",xlabel="",ylabel=""):
        plt.figure(GraphVisualizer.figureNumber)
        GraphVisualizer.figureNumber += 1
        plt.get_current_fig_manager().window.state('zoomed')

        values=values.split("\n")
        keys=keys.split("\n")

        for i in range(len(values)):
            y = []
            x = []
            yPoints=values[i].split(",")
            xPoints= keys[i].split(",")
            for j in range(len(yPoints)):
                if hold=="True" and j>0:
                    y.append(float(yPoints[j-1]))
                    x.append(float(xPoints[j])-0.00000001)
                y.append(float(yPoints[j]))
                x.append(float(xPoints[j]))
            if scatter=="False":
                plt.plot(x, y)
            else:
                plt.scatter(x, y)
            if logscale=="True":
                plt.yscale('log')

            plt.grid(color='grey', linestyle='-', linewidth=0.25)
            plt.legend(legend.split(','))

        plt.title(title)
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
