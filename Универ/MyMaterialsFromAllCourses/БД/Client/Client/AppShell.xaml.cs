using CinemaClientService.Pages;
using Microsoft.Maui.Controls;

namespace CinemaClientService
{
    public partial class AppShell : Shell
    {
        public AppShell()
        {
            InitializeComponent();

            Routing.RegisterRoute(nameof(MainPage), typeof(MainPage));
            Routing.RegisterRoute(nameof(HallsPage), typeof(HallsPage));
            Routing.RegisterRoute(nameof(EmployeeFormPage), typeof(EmployeeFormPage));
            Routing.RegisterRoute(nameof(HallFormPage), typeof(HallFormPage));
            Routing.RegisterRoute(nameof(ClientsPage), typeof(ClientsPage));

            this.Items.Add(new FlyoutItem
            {
                Title = "Сотрудники",
                Icon = "employees_icon.png",
                Route = nameof(MainPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Список сотрудников",
                        ContentTemplate = new DataTemplate(typeof(MainPage))
                    }
                }
            });
            
            this.Items.Add(new FlyoutItem
            {
                Title = "Фильмы",
                Icon = "movies_icon.png",
                Route = nameof(MoviesPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Список фильмов",
                        ContentTemplate = new DataTemplate(typeof(MoviesPage))
                    }
                }
            });

            // Добавление FlyoutItem для залов
            this.Items.Add(new FlyoutItem
            {
                Title = "Залы",
                Icon = "halls_icon.png",
                Route = nameof(HallsPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Список залов",
                        ContentTemplate = new DataTemplate(typeof(HallsPage))
                    }
                }
            });
            
            this.Items.Add(new FlyoutItem
            {
                Title = "Отзывы",
                Icon = "reviews_icon.png",
                Route = nameof(ReviewsPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Список отзывов",
                        ContentTemplate = new DataTemplate(typeof(ReviewsPage))
                    }
                }
            });
            
            this.Items.Add(new FlyoutItem
            {
                Title = "Клиенты",
                Icon = "clients_icon.png",
                Route = nameof(ClientsPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Список клиентов",
                        ContentTemplate = new DataTemplate(typeof(ClientsPage))
                    }
                }
            });
            
            this.Items.Add(new FlyoutItem
            {
                Title = "Места",
                Icon = "reviews_icon.png",
                Route = nameof(SeatsPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Список мест",
                        ContentTemplate = new DataTemplate(typeof(SeatsPage))
                    }
                }
            });
            
            this.Items.Add(new FlyoutItem
            {
                Title = "Сеансы",
                Icon = "sessions_icon.png",
                Route = nameof(SessionsPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Список сеансов",
                        ContentTemplate = new DataTemplate(typeof(SessionsPage))
                    }
                }
            });
            
            this.Items.Add(new FlyoutItem
            {
                Title = "Билеты",
                Icon = "sessions_icon.png",
                Route = nameof(TicketsPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Список билетов",
                        ContentTemplate = new DataTemplate(typeof(TicketsPage))
                    }
                }
            });
            
            this.Items.Add(new FlyoutItem
            {
                Title = "Сеанс_сотрудник",
                Icon = "sessions_icon.png",
                Route = nameof(SessionEmployeesPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Сеанс_сотрудник",
                        ContentTemplate = new DataTemplate(typeof(SessionEmployeesPage))
                    }
                }
            });
            
            this.Items.Add(new FlyoutItem
            {
                Title = "Сеанс_фильм",
                Icon = "sessions_icon.png",
                Route = nameof(SessionMoviesPage),
                Items =
                {
                    new ShellContent
                    {
                        Title = "Сеанс_фильм",
                        ContentTemplate = new DataTemplate(typeof(SessionMoviesPage))
                    }
                }
            });
        }
    }
}