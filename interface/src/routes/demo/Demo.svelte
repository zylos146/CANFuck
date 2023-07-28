<script lang="ts">
	import { onMount, onDestroy } from 'svelte';
	import { user } from '$lib/stores/user';
	import { page } from '$app/stores';
	import { notifications } from '$lib/components/toasts/notifications';
	import SettingsCard from '$lib/components/SettingsCard.svelte';
	import Light from '~icons/tabler/bulb';
	import Info from '~icons/tabler/info-circle';
	import Save from '~icons/tabler/device-floppy';
	import Reload from '~icons/tabler/reload';
	import Alert from '~icons/tabler/alert-triangle-filled';
	import RangeSlider from "svelte-range-slider-pips";

	type StrokeParameters = {
		envelope_start_mm: number;
		envelope_end_mm: number;
		stroke_per_min: number;
		sensation: number;
	};

	let strokeParameters: StrokeParameters = {
		envelope_start_mm: 0,
		envelope_end_mm: 0,
		stroke_per_min: 0,
		sensation: 0
	};

	let motorEnergized = false;
	let strokeRunning = false;

	const ws_token = $page.data.features.security ? '?access_token=' + $user.bearer_token : '';
	const strokeSocket = new WebSocket('ws://' + $page.url.host + '/ws/stroke' + ws_token);
	strokeSocket.onopen = (event) => {
		strokeSocket.send('Hello');
	};

	strokeSocket.onmessage = (event) => {
		const message = JSON.parse(event.data);
		strokeParameters = message.payload
	};

	onDestroy(() => strokeSocket.close());

	function onStrokeChange(e) {
		const [stroke, depth] = e.detail.values
		
		strokeParameters.envelope_start_mm = stroke
		strokeParameters.envelope_end_mm = depth
		strokeSocket.send(JSON.stringify(strokeParameters))
	}

	function onSpeedChange(e) {
		strokeParameters.stroke_per_min = e.detail.value
		strokeSocket.send(JSON.stringify(strokeParameters))
	}

	function onSensationChange(e) {
		strokeParameters.sensation = e.detail.value
		strokeSocket.send(JSON.stringify(strokeParameters))
	}

	function onEnergizeClick() {
		motorEnergized = !motorEnergized
		if (!motorEnergized) {
			strokeRunning = false
		}
	}

	function onRunningClick() {
		if (!motorEnergized) {
			return
		}

		strokeRunning = !strokeRunning
	}
</script>

<style>
	:global(body .rangeSlider) {
		height: initial;
		overflow: initial;
		margin-right: 2em !important;

  	--range-slider:          #646c7c;
		--handle-border:   var(--handle);
		--range-inactive:  var(--handle-focus);
		--range:           var(--handle-focus);
		--float-inactive:  var(--handle-focus);
		--float:           var(--handle-focus);
		--float-text:        white;
		--pip-active:        white;
		--pip-active-text:   var(--pip-active);
	}

:global(body .rangeSlider) {
	--handle1: #00cfff;
	--handle3: #ff2b66;
	--handle2: #3edd8b;
	--handle4: #f5d556;
}

:global(body .rangeSlider:nth-of-type(4)) {
	--handle1: #ff2b66;
}

:global(body .rangeSlider:nth-of-type(6)) {
	--handle1: #f5d556;
}

	:global(.rangeSlider .rangeHandle) {
		--handle-focus: var(--handle);
		--handle-inactive: var(--handle);
		--handle-border: var(--handle);
	}

	:global(.rangeSlider .rangeHandle:nth-of-type(1)) {
		--handle: var(--handle1);
	}
	:global(.rangeSlider .rangeHandle:nth-of-type(2)) {
		--handle: var(--handle2);
	}
	:global(.rangeSlider .rangeHandle:nth-of-type(3)) {
		--handle: var(--handle3);
	}
	:global(.rangeSlider .rangeHandle:nth-of-type(4)) {
		--handle: var(--handle4);
	}

	:global(.rangeSlider .rangeBar) {
		height: 3px;
		background: linear-gradient(90deg, var(--handle1), var(--handle1) 25%, var(--handle2) 75%, var(--handle2) );
	}

	:global(.rangeHandle) {
    width: 15px;
    height: 15px;
    top: 1px;
  }

	:global(.rangeHandle .rangeNub) {
    opacity: 1;
	}

	.machine-stroke {
		transform: rotate(-180deg);

		writing-mode: vertical-rl;
		text-orientation: mixed;
		text-align: center;

		font-weight: 600;
	}

	.nub {
		display: inline-block;

		width: 0.6em;
		height: 1.4em;
		border-radius: 1.4em;

		margin-right: 3px;
		margin-bottom: 5px;
	}

	.nub-depth {
		background-color: #3edd8b;
	}

	.nub-stroke {
		background-color: #00cfff;
	}

	.nub-spm {
		background-color: #ff2b66;
	}

	.nub-sensation {
		background-color: #f5d556;
	}

	.nub-unit {
		margin-top: 5px;
		color: #646c7c;
	}

	.button-container {
		@apply px-5 mt-4 rounded;
		height: 8rem;
	}
	.button-container.inactive {
		@apply bg-green-700;
	}
	.button-container.active {
		@apply bg-red-500;
	}

	.button-container button {
		transform: rotate(-180deg);

		writing-mode: vertical-rl;
		text-align: center;
		
		@apply text-white font-bold;

		width: 2rem;
		font-size: 1.5rem;

		margin-top: 1rem;
		margin-bottom: 1rem;
	}
	
	:global(.button-container svg) {
		transform: rotate(-90deg);

		width: 2rem;
		height: 2rem;
	}
</style>

<div class="flex flex-row flex-1 mt-4 mb-4 ml-4">



	<!-- Stroke / Depth -->
	<div class="flex flex-col w-10">
		<div class="flex-1 machine-stroke">
			<span class="nub nub-depth"/>
			End
			<span class="nub-unit">mm</span>
		</div>
		<div class="flex-1 machine-stroke">Stroke Range</div>
		<div class="flex-1 machine-stroke">
			<span class="nub nub-stroke"/>
			Start
			<span class="nub-unit">mm</span>
		</div>
	</div>

	<RangeSlider 
		min={0}
		max={100}
		values={[0, 50]} 

		range
		vertical 
		pushy

		pips
		pipstep={10}

		first="label" 
		last="label"

		on:change={onStrokeChange}
	/>
	


	<!-- Speed -->
	<div class="flex flex-col w-10">
		<div class="flex-1 machine-stroke">
		</div>
		<div class="flex-1 machine-stroke">
			<span class="nub nub-spm"/>
			Strokes Per Minute</div>
		<div class="flex-1 machine-stroke">
		</div>
	</div>
	<RangeSlider 
		min={0}
		max={100}
		values={[20]} 

		vertical

		pips
		pipstep={10}

		first="label" 
		last="label"
	/>



	<!-- Sensation -->
	<div class="flex flex-col w-10">
		<div class="flex-1 machine-stroke">
			<span class="nub nub-sensation"/>
			Sensation
		</div>
	</div>
	<RangeSlider 
		min={0}
		max={100}
		values={[70]} 

		vertical

		pips
		pipstep={10}

		first="label" 
		last="label"
	/>



	<!-- Motor Control Block -->
	<div class="flex flex-col ml-4 mr-4">
		
		<!-- Motor Control -->
		<div class="flex button-container {motorEnergized ? 'active' : 'inactive'}">
			<!--{#if motorEnergized}<Alert/>{/if}-->
			<button on:click={onEnergizeClick}>
				{motorEnergized ? 'ESTOP' : 'Energize'}
			</button>
			<!--{#if motorEnergized}<Alert/>{/if}-->
		</div>
		<div class="flex button-container {strokeRunning ? 'active' : 'inactive'}">
			<button on:click={onRunningClick}>
				{strokeRunning ? 'Pause' : 'Run'}
			</button>
		</div>

		<!-- Motor Info -->
		<div class="flex flex-row">
			<div class="flex flex-col">
				<div>test</div>
				<div>test</div>
			</div>
			<div class="flex flex-col">
				<div>test</div>
				<div>test</div>
			</div>
		</div>

	</div>



</div>