import networkx as nx
import math

#TODO Generalize renderTopology
class TopologyRenderer:
    @staticmethod
    def renderDynamicPlot(nodesStatic, edgesStatic, nodesDynamic, edgesDynamic, nodePropertyIndex,
                            edgePropertyIndex):
        graph = nx.DiGraph()
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
            graph.add_node(n[0], pos=pos, color=color, label=nodesDynamic[i].split(",")[int(nodePropertyIndex)])
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
            # graph.add_edge(v1id, v2id, weight=edgeWeight, color=color)
            if current > 0:
                graph.add_edge(v1id, v2id, weight=edgeWeight, color=color)
            else:
                graph.add_edge(v2id, v1id, weight=edgeWeight, color=color)

        if len(nodesStatic) > 20:  # TODO this should be a separate  parameter and not be detected by node count
            pos = nx.planar_layout(graph)
        else:
            pos = nx.get_node_attributes(graph, 'pos')
        node_colors = nx.get_node_attributes(graph, 'color')
        node_labels = nx.get_node_attributes(graph, 'label')
        edge_colors = nx.get_edge_attributes(graph, 'color')
        edge_labels = nx.get_edge_attributes(graph, 'weight')
        nx.draw_networkx(graph, pos, labels=node_labels, node_color=node_colors.values(),
                         edge_color=edge_colors.values(), width=1.5, arrowsize=15)
        nx.draw_networkx_edge_labels(graph, pos, edge_labels=edge_labels)

    @staticmethod
    def renderStatic(nodesStatic, edgesStatic, nodePropertyIndex, edgePropertyIndex):
        graph = nx.Graph()
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
            graph.add_node(n[0], pos=pos, color=color, label=n[int(nodePropertyIndex)])

        for e in edgesStatic:
            e = e.split(",")
            graph.add_edge(e[0], e[1], weight=e[int(edgePropertyIndex)], color='black')

        if len(nodesStatic) > 20:  # TODO this should be a separate  parameter and not be detected by node count
            pos = nx.planar_layout(graph)
        else:
            pos = nx.get_node_attributes(graph, 'pos')

        node_colors = nx.get_node_attributes(graph, 'color')
        node_labels = nx.get_node_attributes(graph, 'label')
        edge_colors = nx.get_edge_attributes(graph, 'color')
        labels = nx.get_edge_attributes(graph, 'weight')
        nx.draw_networkx(graph, pos, labels=node_labels, node_color=node_colors.values(),
                         edge_color=edge_colors.values(), width=1.5)
        nx.draw_networkx_edge_labels(graph, pos, edge_labels=labels)