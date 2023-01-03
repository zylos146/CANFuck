<script>
  //import logo from 'assets/img/canfuck-small.png';
</script>
<nav
  class="md:left-0 md:block md:fixed md:top-0 md:bottom-0 md:overflow-y-auto md:flex-row md:flex-nowrap md:overflow-hidden shadow-xl bg-white flex flex-wrap items-center justify-between relative md:w-64 z-10 py-4 px-6"
>
  <div
    class="md:flex-col md:items-stretch md:min-h-full md:flex-nowrap px-0 flex flex-wrap items-center justify-between w-full mx-auto"
  >
    <button
      class="cursor-pointer text-black opacity-50 md:hidden px-3 py-1 text-xl leading-none bg-transparent rounded border border-solid border-transparent"
      type="button"
    >
      <i class="fas fa-bars"></i>
    </button>
    <img src='/assets/img/canfuck-small.png' class="pt-4"/>
    <ul class="md:hidden items-center flex flex-wrap list-none">
      <li class="inline-block relative">
        <div>
          <a class="text-blueGray-500 block py-1 px-3" href="#pablo"
            ><i class="fas fa-bell"></i
          ></a>
          <div
            class="bg-white text-base z-50 float-left py-2 list-none text-left rounded shadow-lg min-w-48 hidden"
          >
            <a
              href="#pablo"
              class="text-sm py-2 px-4 font-normal block w-full whitespace-nowrap bg-transparent text-blueGray-700"
              >Action</a
            >
            <a
              href="#pablo"
              class="text-sm py-2 px-4 font-normal block w-full whitespace-nowrap bg-transparent text-blueGray-700"
              >Another action</a
            >
            <a
              href="#pablo"
              class="text-sm py-2 px-4 font-normal block w-full whitespace-nowrap bg-transparent text-blueGray-700"
              >Something else here</a
            >
            <div
              class="h-0 my-2 border border-solid border-blueGray-100"
            ></div>
            <a
              href="#pablo"
              class="text-sm py-2 px-4 font-normal block w-full whitespace-nowrap bg-transparent text-blueGray-700"
              >Seprated link</a
            >
          </div>
        </div>
      </li>
      <li class="inline-block relative">
        <div>
          <a class="text-blueGray-500 block" href="#pablo"
            ><div class="items-center flex">
              <span
                class="w-12 h-12 text-sm text-white bg-blueGray-200 inline-flex items-center justify-center rounded-full"
                ><img
                  alt="..."
                  class="w-full rounded-full align-middle border-none shadow-lg"
                  src="/notus-svelte/assets/img/team-1-800x800.jpg"
              /></span></div
          ></a>
          <div
            class="bg-white text-base z-50 float-left py-2 list-none text-left rounded shadow-lg min-w-48 hidden"
          >
            <a
              href="#pablo"
              class="text-sm py-2 px-4 font-normal block w-full whitespace-nowrap bg-transparent text-blueGray-700"
              >Action</a
            >
            <a
              href="#pablo"
              class="text-sm py-2 px-4 font-normal block w-full whitespace-nowrap bg-transparent text-blueGray-700"
              >Another action</a
            >
            <a
              href="#pablo"
              class="text-sm py-2 px-4 font-normal block w-full whitespace-nowrap bg-transparent text-blueGray-700"
              >Something else here</a
            >
            <div
              class="h-0 my-2 border border-solid border-blueGray-100"
            ></div>
            <a
              href="#pablo"
              class="text-sm py-2 px-4 font-normal block w-full whitespace-nowrap bg-transparent text-blueGray-700"
              >Seprated link</a
            >
          </div>
        </div>
      </li>
    </ul>
    <div
      class="md:flex md:flex-col md:items-stretch md:opacity-100 md:relative md:mt-4 md:shadow-none shadow absolute top-0 left-0 right-0 z-40 overflow-y-auto overflow-x-hidden h-auto items-center flex-1 rounded hidden"
    >
      <hr class="my-4 md:min-w-full" />
      <h6
        class="md:min-w-full text-blueGray-500 text-xs uppercase font-bold block pt-1 pb-4 no-underline"
      >
        Admin Layout Pages
      </h6>
      <ul class="md:flex-col md:min-w-full flex flex-col list-none">
        <li class="items-center">
          <a
            href="/notus-svelte/admin/dashboard"
            class="text-xs uppercase py-3 font-bold block text-red-500 hover:text-red-600"
            ><i class="fas fa-tv mr-2 text-sm opacity-75"></i> Dashboard</a
          >
        </li>
        <li class="items-center">
          <a
            href="/notus-svelte/admin/settings"
            class="text-xs uppercase py-3 font-bold block text-blueGray-700 hover:text-blueGray-500"
            ><i class="fas fa-tools mr-2 text-sm text-blueGray-300"></i>
            Settings</a
          >
        </li>
        <li class="items-center">
          <a
            href="/notus-svelte/admin/tables"
            class="text-xs uppercase py-3 font-bold block text-blueGray-700 hover:text-blueGray-500"
            ><i class="fas fa-table mr-2 text-sm text-blueGray-300"></i>
            Tables</a
          >
        </li>
        <li class="items-center">
          <a
            href="/notus-svelte/admin/maps"
            class="text-xs uppercase py-3 font-bold block text-blueGray-700 hover:text-blueGray-500"
            ><i class="fas fa-map-marked mr-2 text-sm text-blueGray-300"></i>
            Maps</a
          >
        </li>
      </ul>
      <hr class="my-4 md:min-w-full" />
      <h6
        class="md:min-w-full text-blueGray-500 text-xs uppercase font-bold block pt-1 pb-4 no-underline"
      >
        Auth Layout Pages
      </h6>
      <ul class="md:flex-col md:min-w-full flex flex-col list-none md:mb-4">
        <li class="items-center">
          <a
            class="text-blueGray-700 hover:text-blueGray-500 text-xs uppercase py-3 font-bold block"
            href="/notus-svelte/auth/login"
            ><i class="fas fa-fingerprint text-blueGray-300 mr-2 text-sm"></i>
            Login</a
          >
        </li>
        <li class="items-center">
          <a
            class="text-blueGray-700 hover:text-blueGray-500 text-xs uppercase py-3 font-bold block"
            href="/notus-svelte/auth/register"
            ><i
              class="fas fa-clipboard-list text-blueGray-300 mr-2 text-sm"
            ></i>
            Register</a
          >
        </li>
      </ul>
      <hr class="my-4 md:min-w-full" />
      <h6
        class="md:min-w-full text-blueGray-500 text-xs uppercase font-bold block pt-1 pb-4 no-underline"
      >
        No Layout Pages
      </h6>
      <ul class="md:flex-col md:min-w-full flex flex-col list-none md:mb-4">
        <li class="items-center">
          <a
            class="text-blueGray-700 hover:text-blueGray-500 text-xs uppercase py-3 font-bold block"
            href="/notus-svelte/landing"
            ><i class="fas fa-newspaper text-blueGray-300 mr-2 text-sm"></i>
            Landing Page</a
          >
        </li>
        <li class="items-center">
          <a
            class="text-blueGray-700 hover:text-blueGray-500 text-xs uppercase py-3 font-bold block"
            href="/notus-svelte/profile"
            ><i class="fas fa-user-circle text-blueGray-300 mr-2 text-sm"></i>
            Profile Page</a
          >
        </li>
      </ul>
      <hr class="my-4 md:min-w-full" />
      <h6
        class="md:min-w-full text-blueGray-500 text-xs uppercase font-bold block pt-1 pb-4 no-underline"
      >
        Settings
      </h6>
      <ul class="md:flex-col md:min-w-full flex flex-col list-none md:mb-4">
        <li class="inline-flex">
          <a
            href="https://www.creative-tim.com/learning-lab/tailwind/svelte/colors/notus"
            target="_blank"
            class="text-blueGray-700 hover:text-blueGray-500 text-sm block mb-4 no-underline font-semibold"
            ><i
              class="fas fa-paint-brush mr-2 text-blueGray-300 text-base"
            ></i>
            Styles</a
          >
        </li>

        <li class="inline-flex">
          <a
            href="https://www.creative-tim.com/learning-lab/tailwind/svelte/alerts/notus"
            target="_blank"
            class="text-blueGray-700 hover:text-blueGray-500 text-sm block mb-4 no-underline font-semibold"
            ><i class="fab fa-css3-alt mr-2 text-blueGray-300 text-base"></i>
            CSS Components</a
          >
        </li>

        <li class="inline-flex">
          <a
            href="https://www.creative-tim.com/learning-lab/tailwind/angular/overview/notus"
            target="_blank"
            class="text-blueGray-700 hover:text-blueGray-500 text-sm block mb-4 no-underline font-semibold"
            ><i class="fab fa-angular mr-2 text-blueGray-300 text-base"></i>
            Angular</a
          >
        </li>

        <li class="inline-flex">
          <a
            href="https://www.creative-tim.com/learning-lab/tailwind/js/overview/notus"
            target="_blank"
            class="text-blueGray-700 hover:text-blueGray-500 text-sm block mb-4 no-underline font-semibold"
            ><i class="fab fa-js-square mr-2 text-blueGray-300 text-base"></i>
            Javascript</a
          >
        </li>

        <li class="inline-flex">
          <a
            href="https://www.creative-tim.com/learning-lab/tailwind/nextjs/overview/notus"
            target="_blank"
            class="text-blueGray-700 hover:text-blueGray-500 text-sm block mb-4 no-underline font-semibold"
            ><i class="fab fa-react mr-2 text-blueGray-300 text-base"></i>
            NextJS</a
          >
        </li>

        <li class="inline-flex">
          <a
            href="https://www.creative-tim.com/learning-lab/tailwind/react/overview/notus"
            target="_blank"
            class="text-blueGray-700 hover:text-blueGray-500 text-sm block mb-4 no-underline font-semibold"
            ><i class="fab fa-react mr-2 text-blueGray-300 text-base"></i>
            React</a
          >
        </li>

        <li class="inline-flex">
          <a
            href="https://www.creative-tim.com/learning-lab/tailwind/svelte/overview/notus"
            target="_blank"
            class="text-blueGray-700 hover:text-blueGray-500 text-sm block mb-4 no-underline font-semibold"
            ><i class="fas fa-link mr-2 text-blueGray-300 text-base"></i>
            Svelte</a
          >
        </li>

        <li class="inline-flex">
          <a
            href="https://www.creative-tim.com/learning-lab/tailwind/vue/overview/notus"
            target="_blank"
            class="text-blueGray-700 hover:text-blueGray-500 text-sm block mb-4 no-underline font-semibold"
            ><i class="fab fa-vuejs mr-2 text-blueGray-300 text-base"></i>
            VueJS</a
          >
        </li>
      </ul>
    </div>
  </div>
</nav>