<script lang="ts">
	import { onMount, onDestroy } from 'svelte';
	import { slide } from 'svelte/transition';
	import { cubicOut } from 'svelte/easing';
	import SettingsCard from '$lib/components/SettingsCard.svelte';
	import Collapsible from '$lib/components/Collapsible.svelte';
	import Spinner from '$lib/components/Spinner.svelte';
	import { user } from '$lib/stores/user';
	import { page } from '$app/stores';
	import { notifications } from '$lib/components/toasts/notifications';
	import NTP from '~icons/tabler/clock-check';
	import Server from '~icons/tabler/server';
	import Clock from '~icons/tabler/clock';
	import UTC from '~icons/tabler/clock-pin';
	import Stopwatch from '~icons/tabler/24-hours';

  type SerialStatus = {
    running: boolean
    available: boolean
    connected_ip: string
  }

	type SerialSettings = {
    enabled: boolean;
    baudrate: number;
    rs_config: number;

    port: number;

    rx_pin: number;
    tx_pin: number;
	};

  let serialStatus: SerialStatus;
	let serialSettings: SerialSettings;
  
  // TODO - Should some wifi information be in a global store?
	type WifiStatus = {
		status: number;
		local_ip: string;
		mac_address: string;
		rssi: number;
		ssid: string;
		bssid: string;
		channel: number;
		subnet_mask: string;
		gateway_ip: string;
		dns_ip_1: string;
		dns_ip_2?: string;
	};


	let wifiStatus: WifiStatus;

	let formErrors = {
		baudrate: false,
		port: false,
		rx_pin: false,
		tx_pin: false
	};

  async function getStatus() {
    await Promise.all([getWifiStatus(), getSerialStatus(), getSerialSettings()]);
  }

	async function getWifiStatus() {
		try {
			const response = await fetch('/rest/wifiStatus', {
				method: 'GET',
				headers: {
					Authorization: $page.data.features.security ? 'Bearer ' + $user.bearer_token : 'Basic',
					'Content-Type': 'application/json'
				}
			});
			wifiStatus = await response.json();
		} catch (error) {
			console.error('Error:', error);
		}
		return wifiStatus;
	}

	const interval2 = setInterval(async () => {
		getWifiStatus();
	}, 5000);

	async function getSerialStatus() {
		try {
			const response = await fetch('/rest/serial/status', {
				method: 'GET',
				headers: {
					Authorization: $page.data.features.security ? 'Bearer ' + $user.bearer_token : 'Basic',
					'Content-Type': 'application/json'
				}
			});
			serialStatus = await response.json();
		} catch (error) {
			console.error('Error:', error);
		}
		return;
	}

	async function getSerialSettings() {
		try {
			const response = await fetch('/rest/serial/settings', {
				method: 'GET',
				headers: {
					Authorization: $page.data.features.security ? 'Bearer ' + $user.bearer_token : 'Basic',
					'Content-Type': 'application/json'
				}
			});
			serialSettings = await response.json();
		} catch (error) {
			console.error('Error:', error);
		}
		return;
	}

	const interval = setInterval(async () => {
		getSerialStatus();
	}, 5000);

	onDestroy(() => clearInterval(interval));

	onMount(() => {
		if (!$page.data.features.security || $user.admin) {
			getSerialSettings();
		}
	});

	let formField: any;

	async function postSerialSettings(data: SerialSettings) {
		try {
			const response = await fetch('/rest/serial/settings', {
				method: 'POST',
				headers: {
					Authorization: $page.data.features.security ? 'Bearer ' + $user.bearer_token : 'Basic',
					'Content-Type': 'application/json'
				},
				body: JSON.stringify(data)
			});

			if (response.status == 200) {
				notifications.success('Security settings updated.', 3000);
				serialSettings = await response.json();
			} else {
				notifications.error('User not authorized.', 3000);
			}
		} catch (error) {
			console.error('Error:', error);
		}
	}

	function handleSubmitSerial() {
    // TODO - Validate
    postSerialSettings(serialSettings)

    /*
		let valid = true;

		// Validate Server
		// RegEx for IPv4
		const regexExpIPv4 =
			/\b(?:(?:2(?:[0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9])\.){3}(?:(?:2([0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9]))\b/;
		const regexExpURL =
			/[-a-zA-Z0-9@:%_\+.~#?&//=]{2,256}\.[a-z]{2,4}\b(\/[-a-zA-Z0-9@:%_\+.~#?&//=]*)?/i;

		if (!regexExpURL.test(ntpSettings.server) && !regexExpIPv4.test(ntpSettings.server)) {
			valid = false;
			formErrors.server = true;
		} else {
			formErrors.server = false;
		}

		ntpSettings.tz_format = TIME_ZONES[ntpSettings.tz_label];

		// Submit JSON to REST API
		if (valid) {
			postNTPSettings(ntpSettings);
			//alert('Form Valid');
		}
    */
	}
</script>

<SettingsCard collapsible={false}>
	<Clock slot="icon" class="lex-shrink-0 mr-2 h-6 w-6 self-end" />
	<span slot="title">Remote Serial</span>
	<div class="w-full overflow-x-auto">
		{#await getStatus()}
			<Spinner />
		{:then nothing}
			<div
				class="flex w-full flex-col space-y-1"
				transition:slide|local={{ duration: 300, easing: cubicOut }}
			>
				<div class="rounded-box bg-base-100 flex items-center space-x-3 px-4 py-2">
					<div
						class="mask mask-hexagon h-auto w-10 {serialStatus.available
							? 'bg-success'
							: 'bg-error'}"
					>
						<NTP
							class="h-auto w-full scale-75 {serialStatus.available
								? 'text-success-content'
								: 'text-error-content'}"
						/>
					</div>
					<div>
						<div class="font-bold">Status</div>
						<div class="text-sm opacity-75">
              {#if serialStatus.running}
							  {serialStatus.available ? 'Available' : 'Active'}
              {:else}
                Disabled
              {/if}
						</div>
					</div>
				</div>

				<div class="rounded-box bg-base-100 flex items-center space-x-3 px-4 py-2">
					<div class="mask mask-hexagon bg-primary h-auto w-10">
						<Server class="text-primary-content h-auto w-full scale-75" />
					</div>
					<div>
						<div class="font-bold">ESP32 Connection String</div>
						<div class="text-sm opacity-75">
							{wifiStatus.local_ip} : {serialSettings.port}
						</div>
					</div>
				</div>

        {#if !serialStatus.available}
          <div class="rounded-box bg-base-100 flex items-center space-x-3 px-4 py-2">
            <div class="mask mask-hexagon bg-primary h-auto w-10">
              <Clock class="text-primary-content h-auto w-full scale-75" />
            </div>
            <div>
              <div class="font-bold">Connected Client IP</div>
              <div class="text-sm opacity-75">
                {serialStatus.connected_ip}
              </div>
            </div>
          </div>
        {/if}
			</div>
		{/await}
	</div>

	{#if !$page.data.features.security || $user.admin}
		<Collapsible open={false} class="shadow-lg" on:closed={getSerialSettings}>
			<span slot="title">Change Serial Settings</span>
      
			<form on:submit|preventDefault={handleSubmitSerial} novalidate bind:this={formField}>
				<div class="grid w-full grid-cols-1 content-center gap-x-4 px-4 sm:grid-cols-2">
        
					<!-- Port -->
					<div>
						<label class="label" for="port">
							<span class="label-text text-md">Port</span>
						</label>
						<input
							type="number"
							min="0"
							max="65536"
							class="input input-bordered invalid:border-error w-full invalid:border-2 {formErrors.port
								? 'border-error border-2'
								: ''}"
							bind:value={serialSettings.port}
							id="port"
							required
						/>
						<label for="port" class="label"
							><span class="label-text-alt text-error {formErrors.port ? '' : 'hidden'}"
								>Port number must be between 0 and 65536</span
							></label
						>
					</div>
          
					<!-- Baud Rate -->
					<div>
						<label class="label" for="baudrate">
							<span class="label-text text-md">Baud Rate</span>
						</label>
						<label for="baudrate" class="input-group">
							<input
								type="number"
								min="1"
								max="600"
								class="input input-bordered invalid:border-error w-full invalid:border-2 {formErrors.baudrate
									? 'border-error border-2'
									: ''}"
								bind:value={serialSettings.baudrate}
								id="baudrate"
								required
							/>
							<span>Seconds</span>
						</label>
						<label for="baudrate" class="label"
							><span class="label-text-alt text-error {formErrors.baudrate ? '' : 'hidden'}"
								>Must be between 1 and 600 seconds</span
							></label
						>
					</div>

					<!-- Receive Pin -->
					<div>
						<label class="label" for="rx_pin">
							<span class="label-text text-md">Receive Pin</span>
						</label>
						<label for="rx_pin" class="input-group">
							<input
								type="number"
								min="1"
								max="600"
								class="input input-bordered invalid:border-error w-full invalid:border-2 {formErrors.rx_pin
									? 'border-error border-2'
									: ''}"
								bind:value={serialSettings.rx_pin}
								id="rx_pin"
								required
							/>
							<span>Pin</span>
						</label>
						<label for="rx_pin" class="label"
							><span class="label-text-alt text-error {formErrors.rx_pin ? '' : 'hidden'}"
								>Must be between TODO</span
							></label
						>
					</div>

					<!-- Transmit Pin -->
					<div>
						<label class="label" for="tx_pin">
							<span class="label-text text-md">Transmit Pin</span>
						</label>
						<label for="tx_pin" class="input-group">
							<input
								type="number"
								min="1"
								max="600"
								class="input input-bordered invalid:border-error w-full invalid:border-2 {formErrors.tx_pin
									? 'border-error border-2'
									: ''}"
								bind:value={serialSettings.tx_pin}
								id="tx_pin"
								required
							/>
							<span>Pin</span>
						</label>
						<label for="tx_pin" class="label"
							><span class="label-text-alt text-error {formErrors.tx_pin ? '' : 'hidden'}"
								>Must be between TODO</span
							></label
						>
					</div>

          <label class="label cursor-pointer justify-start gap-4">
            <input
              type="checkbox"
              bind:checked={serialSettings.enabled}
              class="checkbox checkbox-primary"
            />
            <span class="">Enable Serial</span>
          </label>

          <div class="mt-6 place-self-end">
            <button class="btn btn-primary" type="submit">Apply Settings</button>
          </div>
        </div>
			</form>
		</Collapsible>
	{/if}
</SettingsCard>
