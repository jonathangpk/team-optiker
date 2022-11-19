import { useMemo } from "react"
import ReactApexChart from 'react-apexcharts'
import { useStore } from "../state/state"

interface PriceChartProps {
  priceHistory: {[key: number]: number}
}
export function PriceChart(props: PriceChartProps) {
  const store = useStore()
  console.log(store.authenticated)
  const data = useMemo(() => {
    return [
      {
        x: new Date('2021-01-01'),
        y: 0
      },
      {
        x: new Date('2021-01-02'),
        y: 1
      },
      {
        x: new Date('2021-01-03'),
        y: 2
      },
      {
        x: new Date('2021-01-04'),
        y: 3
      },
  ]
  }, [props.priceHistory])
  return (
    <div>
      <h1>Price Chart</h1>
      <ReactApexChart
        options={{
          chart: {
            type: 'area',
            stacked: false,
            height: 350,
            zoom: {
              type: 'x',
              enabled: true,
              autoScaleYaxis: true
            },
            toolbar: {
              autoSelected: 'zoom'
            }
          },
          dataLabels: {
            enabled: false
          },
          markers: {
            size: 0,
          },
          title: {
            text: 'Stock Price Movement',
            align: 'left'
          },
          fill: {
            type: 'gradient',
            gradient: {
              shadeIntensity: 1,
              inverseColors: false,
              opacityFrom: 0.5,
              opacityTo: 0,
              stops: [0, 90, 100]
            },
          },
          yaxis: {
            labels: {
              formatter: function (val) {
                return (val / 1000000).toFixed(0);
              },
            },
            title: {
              text: 'Price'
            },
          },
          xaxis: {
            type: 'datetime',
          },
          tooltip: {
            shared: false,
            y: {
              formatter: function (val) {
                return (val / 1000000).toFixed(0)
              }
            }
          }
        }}
        series={[{
          name: 'Price',
          data,
        }]}
        type="area"
        height={350} />
    </div>
  )
}