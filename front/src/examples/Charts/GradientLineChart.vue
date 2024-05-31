<template>
  <div class="card">
    <div class="pb-0 card-header mb-0">
      <h6>{{ title }}</h6>
      <p class="text-sm">
        <i class="fa fa-arrow-up text-success"></i>
        <span class="font-weight-bold">{{ detail1 }}</span>
        {{ detail2 }}
      </p>
      <!-- Dropdown for selecting data type -->
      <select v-model="selectedDataType" @change="handleSelectionChange">
        <option value="temperature">Temperature</option>
        <option value="alcohol">Alcohol</option>
        <option value="humidity">Humidity</option>
        <option value="nh4">Amonium</option>
        <option value="acetona">Acetone</option>
        <option value="co">Carbon Monoxide</option>
        <option value="tolueno">Tolueno</option>
        <option value="co2">Carbon Dioxide</option>
      </select>
    </div>
    <div class="p-3 card-body">
      <div class="chart">
        <canvas id="chart-line" class="chart-canvas" height="300"></canvas>
      </div>
    </div>
  </div>
</template>

<script>
import Chart from "chart.js/auto";
import axios from 'axios';

export default {
  name: "gradient-line-chart",
  data() {
    return {
      devices: [],
      temperature_values: [],
      device_types: [],
      selectedDataType: 'temperature',  // default value
      chart: null  // reference to the chart instance
    };
  },
  props: {
    title: {
      type: String,
      default: "Device Information",
    },
    detail1: {
      type: String,
      default: "Values are",
    },
    detail2: {
      type: String,
      default: " shown in their own scales.",
    },
  },
  async mounted() {
    await this.fetchData(this.selectedDataType);
  },
  methods: {
    async fetchData(dataType) {
      try {
        const response = await axios.get('http://127.0.0.1:8000/all_items?limit=25');
        const data = JSON.parse(response.data);
        this.devices = data;
        
        this.temperature_values = [];
        this.device_types = [];

        for (let i = 0; i < this.devices.length; i++) {
          if (Object.prototype.hasOwnProperty.call(this.devices[i], dataType)) {
            this.temperature_values.push(this.devices[i][dataType]);
            this.device_types.push(this.devices[i]['id']);
          }
        }

        this.renderChart(dataType);
      } catch (error) {
        console.error('Error fetching device data:', error);
      }
    },
    renderChart(dataType) {
      if (this.chart) {
        this.chart.destroy();
      }
      
      var ctx1 = document.getElementById("chart-line").getContext("2d");

      var gradientStroke1 = ctx1.createLinearGradient(0, 230, 0, 50);
      gradientStroke1.addColorStop(1, "rgba(94, 114, 228, 0.2)");
      gradientStroke1.addColorStop(0.2, "rgba(94, 114, 228, 0.0)");
      gradientStroke1.addColorStop(0, "rgba(94, 114, 228, 0)");

      this.chart = new Chart(ctx1, {
        type: "line",
        data: {
          labels: this.device_types,
          datasets: [
            {
              label: dataType.charAt(0).toUpperCase() + dataType.slice(1),
              tension: 0.4,
              borderWidth: 3,
              pointRadius: 0,
              borderColor: "#4BB543 ",
              backgroundColor: gradientStroke1,
              fill: true,
              data: this.temperature_values,
              maxBarThickness: 6,
            },
          ],
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          plugins: {
            legend: {
              display: false,
            },
          },
          interaction: {
            intersect: false,
            mode: "index",
          },
          scales: {
            y: {
              grid: {
                drawBorder: false,
                display: true,
                drawOnChartArea: true,
                drawTicks: false,
                borderDash: [5, 5],
              },
              ticks: {
                display: true,
                padding: 10,
                color: "#fbfbfb",
                font: {
                  size: 11,
                  family: "Open Sans",
                  style: "normal",
                  lineHeight: 2,
                },
              },
            },
            x: {
              grid: {
                drawBorder: false,
                display: false,
                drawOnChartArea: false,
                drawTicks: false,
                borderDash: [5, 5],
              },
              ticks: {
                display: true,
                color: "#ccc",
                padding: 20,
                font: {
                  size: 11,
                  family: "Open Sans",
                  style: "normal",
                  lineHeight: 2,
                },
              },
            },
          },
        },
      });
    },
    handleSelectionChange() {
      const dataType = this.selectedDataType;
      this.updateProps(dataType);
      this.fetchData(dataType);
    },
    updateProps(dataType) {
      switch (dataType) {
        case 'temperature':
          this.$props.title = "Device Temperatures";
          this.$props.detail1 = "Temperatures are";
          this.$props.detail2 = " shown in °C";
          break;
        case 'alcohol':
          this.$props.title = "Device Alcohol Levels";
          this.$props.detail1 = "Alcohol levels are";
          this.$props.detail2 = " shown in %";
          break;
        case 'humidity':
          this.$props.title = "Device Humidity Levels";
          this.$props.detail1 = "Humidity levels are";
          this.$props.detail2 = " shown in %";
          break;
        case 'no2':
          this.$props.title = "Device NO2 Levels";
          this.$props.detail1 = "NO2 levels are";
          this.$props.detail2 = " shown in ppm";
          break;
        default:
          break;
      }
    }
  }
};
</script>

<style scoped></style>
