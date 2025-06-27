namespace CinemaClientService.ViewModels
{
    public class SessionEmployeeView
    {
        public int SessionId { get; set; }
        public int EmployeeId { get; set; }
        public string SessionInfo { get; set; } = string.Empty;  
        public string EmployeeInfo { get; set; } = string.Empty;
    }
}