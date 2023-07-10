<SettingsCard collapsible={false}>
	<Clock slot="icon" class="lex-shrink-0 mr-2 h-6 w-6 self-end" />
	<span slot="title">Network Time</span>
	<div class="w-full overflow-x-auto">
		{#await getNTPStatus()}
			<Spinner />
		{:then nothing}
			<div
				class="flex w-full flex-col space-y-1"
				transition:slide|local={{ duration: 300, easing: cubicOut }}
			>
				<div class="rounded-box bg-base-100 flex items-center space-x-3 px-4 py-2">
					<div
						class="mask mask-hexagon h-auto w-10 {ntpStatus.status === 1
							? 'bg-success'
							: 'bg-error'}"
					>
						<NTP
							class="h-auto w-full scale-75 {ntpStatus.status === 1
								? 'text-success-content'
								: 'text-error-content'}"
						/>
					</div>
					<div>
						<div class="font-bold">Status</div>
						<div class="text-sm opacity-75">
							{ntpStatus.status === 1 ? 'Active' : 'Inactive'}
						</div>
					</div>
				</div>

				<div class="rounded-box bg-base-100 flex items-center space-x-3 px-4 py-2">
					<div class="mask mask-hexagon bg-primary h-auto w-10">
						<Server class="text-primary-content h-auto w-full scale-75" />
					</div>
					<div>
						<div class="font-bold">NTP Server</div>
						<div class="text-sm opacity-75">
							{ntpStatus.server}
						</div>
					</div>
				</div>

				<div class="rounded-box bg-base-100 flex items-center space-x-3 px-4 py-2">
					<div class="mask mask-hexagon bg-primary h-auto w-10">
						<Clock class="text-primary-content h-auto w-full scale-75" />
					</div>
					<div>
						<div class="font-bold">Local Time</div>
						<div class="text-sm opacity-75">
							{new Intl.DateTimeFormat('en-GB', {
								dateStyle: 'long',
								timeStyle: 'long'
							}).format(new Date(ntpStatus.local_time))}
						</div>
					</div>
				</div>

				<div class="rounded-box bg-base-100 flex items-center space-x-3 px-4 py-2">
					<div class="mask mask-hexagon bg-primary h-auto w-10">
						<UTC class="text-primary-content h-auto w-full scale-75" />
					</div>
					<div>
						<div class="font-bold">UTC Time</div>
						<div class="text-sm opacity-75">
							{new Intl.DateTimeFormat('en-GB', {
								dateStyle: 'long',
								timeStyle: 'long',
								timeZone: 'UTC'
							}).format(new Date(ntpStatus.utc_time))}
						</div>
					</div>
				</div>

				<div class="rounded-box bg-base-100 flex items-center space-x-3 px-4 py-2">
					<div class="mask mask-hexagon bg-primary h-auto w-10">
						<Stopwatch class="text-primary-content h-auto w-full scale-75" />
					</div>
					<div>
						<div class="font-bold">Uptime</div>
						<div class="text-sm opacity-75">
							{convertSeconds(ntpStatus.uptime)}
						</div>
					</div>
				</div>
			</div>
		{/await}
	</div>

	{#if !$page.data.features.security || $user.admin}
		<Collapsible open={false} class="shadow-lg" on:closed={getNTPSettings}>
			<span slot="title">Change NTP Settings</span>
			<form
				class="form-control w-full"
				on:submit|preventDefault={handleSubmitNTP}
				novalidate
				bind:this={formField}
			>
				<label class="label cursor-pointer justify-start gap-4">
					<input
						type="checkbox"
						bind:checked={ntpSettings.enabled}
						class="checkbox checkbox-primary"
					/>
					<span class="">Enable NTP</span>
				</label>
				<label class="label" for="server">
					<span class="label-text text-md">Server</span>
				</label>
				<input
					type="text"
					min="3"
					max="64"
					class="input input-bordered invalid:border-error w-full invalid:border-2 {formErrors.server
						? 'border-error border-2'
						: ''}"
					bind:value={ntpSettings.server}
					id="server"
					required
				/>
				<label class="label" for="subnet">
					<span class="label-text-alt text-error {formErrors.server ? '' : 'hidden'}"
						>Must be a valid IPv4 address or URL</span
					>
				</label>
				<label class="label" for="tz">
					<span class="label-text text-md">Pick Time Zone</span>
				</label>
				<select class="select select-bordered" bind:value={ntpSettings.tz_label} id="tz">
					{#each Object.entries(TIME_ZONES) as [tz_label, tz_format]}
						<option value={tz_label}>{tz_label}</option>
					{/each}
				</select>

				<div class="mt-6 place-self-end">
					<button class="btn btn-primary" type="submit">Apply Settings</button>
				</div>
			</form>
		</Collapsible>
	{/if}
</SettingsCard>
