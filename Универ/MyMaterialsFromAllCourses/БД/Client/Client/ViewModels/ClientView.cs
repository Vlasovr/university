// ViewModels/ClientView.cs
namespace CinemaClientService.ViewModels
{
    public class ClientView
    {
        public int ClientId { get; set; }
        public string Email { get; set; }
        public string FullName { get; set; }
        public string OrderHistory { get; set; }
    }
}