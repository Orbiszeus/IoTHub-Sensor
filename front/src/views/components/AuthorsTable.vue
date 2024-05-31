<template>
  <div class="card">
    <div class="card-header pb-0">
      <h6>Broker Information</h6>
      <!-- Dropdown for selecting limit -->
      <select v-model="selectedLimit" @change="handleLimitChange" class="custom-select">
        <option v-for="n in 101" :key="n" :value="n">{{ n }}</option>
      </select>
    </div>
    <div class="card-body px-0 pt-0 pb-2">
      <div class="table-responsive p-0">
        <table class="table align-items-center mb-0">
          <thead>
            <tr>
              <th id="x" class="text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">ID</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Temperature</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Humidity</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">CO</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Toluene</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">CO2</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Alcohol</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Acetone</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">NH4</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Hour</th>
              <th class="text-secondary opacity-7"></th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(device, index) in this.devices" :key="index">
              <td>
                <div class="d-flex px-2 py-1">
                  <div>
                    <div class="form-check form-switch">
                      <label class="form-check-label" :for="id">
                        <slot />
                      </label>
                    </div>
                  </div>
                  <div class="d-flex flex-column justify-content-center">
                    <h6 class="mb-0 text-sm">{{ device.id }}</h6>
                  </div>
                </div>
              </td>
              <td class="align-middle text-center text-sm">
                <p class="text-xs font-weight-bold mb-0">{{ device.temperature }}</p>
              </td>
              <td class="align-middle text-center text-sm">
                <p class="text-xs text-secondary mb-0">{{ device.humidity }}</p>
              </td>
              
              <td class="align-middle text-center text-sm">
                <p class="text-xs text-secondary mb-0">{{ device.co }}</p>
              </td>
              <td class="align-middle text-center text-sm">
                <p class="text-xs text-secondary mb-0">{{ device.tolueno }}</p>
              </td>
              <td class="align-middle text-center text-sm">
                <p class="text-xs text-secondary mb-0">{{ device.co2 }}</p>
              </td>
              <td class="align-middle text-center text-sm">
                <p class="text-xs text-secondary mb-0">{{ device.alcohol }}</p>
              </td>
              <td class="align-middle text-center text-sm">
                <p class="text-xs text-secondary mb-0">{{ device.acetona }}</p>
              </td>
              <td class="align-middle text-center text-sm">
                <p class="text-xs text-secondary mb-0">{{ device.nh4 }}</p>
              </td>
              <td class="align-middle text-center">
                <span class="text-secondary text-xs font-weight-bold">{{ device['date'].toString().slice(0, 9) }}</span>
              </td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: "authors-table",
  data() {
    return {
      devices: [],
      selectedLimit: 20, // default limit
    };
  },
  mounted() {
    this.fetchDeviceData();
  },
  methods: {
    async fetchDeviceData() {
      try {
        const response = await axios.get(`http://127.0.0.1:8000/all_items?limit=${this.selectedLimit}`);
        const data = JSON.parse(response.data);
        console.log('Device data:', data);
        this.devices = data;
      } catch (error) {
        console.error('Error fetching device data:', error);
      }
    },
    handleLimitChange() {
      this.fetchDeviceData();
    },
  }
};
</script>

<style>
.form-check {
  padding-right: 10px;
}
#x {
  padding-left: 90px !important;
}

.custom-select {
  border: 1px solid #ced4da !important;
  border-radius: 0.25rem !important;
  padding: 0.375rem 1.75rem 0.375rem 0.75rem;
  font-size: 1rem;
  line-height: 1.5;
  background-color: #fff;
  background-clip: padding-box;
  box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.075);
  transition: border-color 0.15s ease-in-out, box-shadow 0.15s ease-in-out !important;
}

.custom-select:focus {
  border-color: #80bdff;
  outline: 0;
  box-shadow: 0 0 0 0.2rem rgba(0, 123, 255, 0.25);
}
</style>
