import matplotlib.pyplot as plt

class CurveRenderer:
    @staticmethod
    def renderCurves(keys, values, scatter="False", hold="False", logscale="False", legend="", title="", xlabel="",ylabel=""):

        for i in range(len(values)):
            y = []
            x = []
            yPoints = values[i].split(",")
            xPoints = keys[i].split(",")
            for j in range(len(yPoints)):
                if hold == "True" and j > 0:
                    y.append(float(yPoints[j - 1]))
                    x.append(float(xPoints[j]) - 0.00000001)
                y.append(float(yPoints[j]))
                x.append(float(xPoints[j]))
            if scatter == "False":
                plt.plot(x, y)
            else:
                plt.scatter(x, y)
            if logscale == "True":
                plt.yscale('log')

            plt.grid(color='grey', linestyle='-', linewidth=0.25)
            plt.legend(legend.split(','))

        plt.title(title)
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)